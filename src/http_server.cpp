#include <cerrno>
#include <fstream>
#include <http_server.hpp>
#include <optional>
#include <utils.hpp>

#include <fcntl.h>

namespace shttp {

namespace {

void setNonBlocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

} // namespace

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

  if (bind(server_fd_, (sockaddr *)(&addr), addrlen) < 0) {
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
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = server_fd_;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, server_fd_, &ev);
}

void HttpServer::acceptClientConnection() {
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  file_descriptor_t client_fd =
      accept(server_fd_, (sockaddr *)&client_addr, &client_len);
  setNonBlocking(client_fd);

  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = client_fd;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &ev);
}

std::string HttpServer::recv_request(size_t client_fd) {
  std::string recv_buffer;
  std::string buffer(BUFFER_SIZE, '\0');
  size_t buffer_size = 0;
  while (true) {
    size_t bytes_read = recv(client_fd, buffer.data(), BUFFER_SIZE, 0);
    if (bytes_read > 0) {
      recv_buffer.append(buffer.data(), bytes_read);
      if (recv_buffer.find("\r\n\r\n") != std::string::npos) {
        break;
      }
    } else if (bytes_read == 0) {
      close(client_fd);
      break;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        perror("recv");
        close(client_fd);
        break;
      }
    }
  }
  return recv_buffer;
}

void HttpServer::proccesRequest(size_t client_fd) {
  HttpRequest request = recv_request(client_fd);
  HttpResponse response;
  bool served = false;
  if (!static_root_.empty()) {
    served = handleStaticFiles(request, response);
  }
  if (!served) {
    request_handler_(request, response);
  }

  std::string response_str = response.toString();
  size_t total_sent = 0;
  while (total_sent < response_str.size()) {
    size_t bytes_sent = send(client_fd, response_str.c_str() + total_sent,
                             response_str.size() - total_sent, 0);

    if (bytes_sent == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        perror("send");
        close(client_fd);
        return;
      }
    }

    total_sent += bytes_sent;
  }
}

void HttpServer::startEpollLoop() {
  using namespace std::chrono_literals;
  for (;;) {
    size_t event_count = epoll_wait(epoll_fd_, events, MAX_EVENTS, 0);

    for (size_t i = 0; i < event_count; ++i) {
      int fd = events[i].data.fd;
      if (fd == server_fd_) {
        acceptClientConnection();
      } else {
        thread_pool_.submit([this, fd]() { this->proccesRequest(fd); });
      }
    }
    std::this_thread::sleep_for(10ns);
  }
}

bool HttpServer::handleStaticFiles(const HttpRequest &request,
                                   HttpResponse &response) {
  if (request.getMethod() != HttpMethod::GET)
    return false;

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

} // namespace shttp
