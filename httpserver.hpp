#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

namespace shttp {

constexpr size_t PORT = 8080;
constexpr size_t BUFFER_SIZE = 1024 * 8;
constexpr size_t MAX_EVENTS = 10;
constexpr size_t MAX_TIMEOUT = 30'000;

using file_descriptor_t = int;
using HttpRequest = std::string;
using HttpResponse = std::string;
using ResponseCallback = std::function<HttpResponse(const HttpRequest&)>;

namespace {
void set_non_blocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

struct HttpContext {};
}  // namespace

enum class HttpServerStatus { OK, SOCKET_ERROR, BIND_ERROR, LISTEN_ERROR };

struct HttpServerResult {
  HttpServerStatus status;
  std::string error_message;
};

// TCP connection
class http_server {
 public:
  http_server() = default;

  http_server(const http_server&) = delete;
  http_server& operator=(const http_server&) = delete;
  http_server(http_server&&) = delete;
  http_server& operator=(http_server&&) = delete;

  ~http_server() {
    close(server_fd_);
    close(epoll_fd_);
  }

  void setRequestHandler(ResponseCallback cb) {
    request_handler_ = std::move(cb);
  }

  HttpServerResult run() {
    auto start_server_res = start_server();
    if (start_server_res.status != HttpServerStatus::OK) {
      return start_server_res;
    }

    configure_epoll();

    start_epoll_loop();

    return {HttpServerStatus::OK, "Server is running"};
  }

 private:
  ResponseCallback request_handler_ =
      [](const HttpRequest& request) -> HttpResponse {
    std::cout << request << '\n';
    return request;
  };
  file_descriptor_t server_fd_;
  file_descriptor_t epoll_fd_;
  epoll_event ev{}, events[MAX_EVENTS];

 private:
  HttpServerResult start_server() noexcept {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd_ < 0) {
      return {HttpServerStatus::SOCKET_ERROR, "Failed to create socket"};
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
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

    set_non_blocking(server_fd_);

    return {HttpServerStatus::OK, "Server started successfully"};
  }

  void configure_epoll() {
    epoll_fd_ = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = server_fd_;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, server_fd_, &ev);
  }

  void accept_client_connection() {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    file_descriptor_t client_fd =
        accept(server_fd_, (sockaddr*)&client_addr, &client_len);
    set_non_blocking(client_fd);

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = client_fd;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &ev);
  }

  void procces_request(size_t event_index) {
    file_descriptor_t client_fd = events[event_index].data.fd;
    char buffer[BUFFER_SIZE];
    size_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_read <= 0) {
      close(client_fd);
    } else {
      HttpRequest request(buffer, bytes_read);
      HttpResponse response = request_handler_(request);
      send(client_fd, response.c_str(), response.size(), 0);
    }
  }

  void start_epoll_loop() {
    using namespace std::chrono_literals;
    for (;;) {
      size_t event_count =
          epoll_wait(epoll_fd_, events, MAX_EVENTS, MAX_TIMEOUT);

      for (size_t i = 0; i < event_count; ++i) {
        if (events[i].data.fd == server_fd_) {
          accept_client_connection();
        } else {
          procces_request(i);
        }
      }
      std::this_thread::sleep_for(10ns);
    }
  }
};

}  // namespace shttp

#endif
