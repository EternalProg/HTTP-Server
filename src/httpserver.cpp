#include <fstream>
#include <httpserver.hpp>
#include <optional>
#include <utils.hpp>

namespace shttp {

HttpServer::HttpServer(uint16_t port, uint16_t thread_count)
    : port_(port), thread_pool_(thread_count) {}

HttpServerResult HttpServer::run() {
  auto startServer_res = startServer(port_);
  if (startServer_res.status != HttpServerStatus::OK) {
    return startServer_res;
  }

  configureEpoll();

  startEpollLoop();

  return {HttpServerStatus::OK, "Server is running"};
}

HttpServerResult HttpServer::startServer(uint16_t port) noexcept {
  server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd_ < 0) {
    return {HttpServerStatus::SOCKET_ERROR, "Failed to create socket"};
  }

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  socklen_t addrlen = sizeof(addr);

  if (bind(server_fd_, (sockaddr*)(&addr), addrlen) < 0) {
    close(server_fd_);
    return {HttpServerStatus::BIND_ERROR, "Failed to bind socket"};
  }

  if (listen(server_fd_, 3) < 0) {
    close(server_fd_);
    return {HttpServerStatus::LISTEN_ERROR, "Failed to listen on socket"};
  }

  setNonBlocking(server_fd_);

  return {HttpServerStatus::OK, "Server started successfully"};
}

void HttpServer::configureEpoll() {
  epoll_fd_ = epoll_create1(0);
  ev.events = EPOLLIN;
  ev.data.fd = server_fd_;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, server_fd_, &ev);
}

void HttpServer::acceptClientConnection() {
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  file_descriptor_t client_fd =
      accept(server_fd_, (sockaddr*)&client_addr, &client_len);
  setNonBlocking(client_fd);

  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = client_fd;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &ev);
}

void HttpServer::proccesRequest(size_t client_fd) {
  char buffer[BUFFER_SIZE];
  size_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_read < 0) {
    perror("recv");
    close(client_fd);
  } else if (bytes_read == 0) {
    close(client_fd);
  } else {
    HttpRequest request(buffer, bytes_read);
    HttpResponse response;
    bool served = false;
    if (!static_root_.empty()) {
      served = handleStaticFiles(request, response);
    }
    if (!served) {
      request_handler_(request, response);
    }

    std::string response_str = response.toString();
    send(client_fd, response_str.c_str(), response_str.size(), 0);
  }
}

void HttpServer::startEpollLoop() {
  using namespace std::chrono_literals;
  for (;;) {
    size_t event_count = epoll_wait(epoll_fd_, events, MAX_EVENTS, 0);

    for (size_t i = 0; i < event_count; ++i) {
      if (events[i].data.fd == server_fd_) {
        acceptClientConnection();
      } else {
        file_descriptor_t client_fd = events[i].data.fd;
        thread_pool_.submit(
            [this, client_fd]() { this->proccesRequest(client_fd); });
      }
    }
    std::this_thread::sleep_for(10ns);
  }
}

bool HttpServer::handleStaticFiles(const HttpRequest& request,
                                   HttpResponse& response) {
  if (request.getMethod() != HttpMethod::GET) return false;

  Path full_path = static_root_ + utils::sanitizePath(request.getPath());
  std::optional<std::string> body = utils::readFromFile(full_path);

  response.setBody(body.value_or("<h1>404 Not Found</h1>"));

  if (body.has_value()) {
    response.setStatus(HttpStatusCode::OK);
    response.setHeader("Content-Type", utils::getMimeType(full_path));
    return true;
  }
  response.setStatus(HttpStatusCode::NOT_FOUND);
  return false;
}

}  // namespace shttp