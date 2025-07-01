#ifndef SHTTP_SERVER_HPP
#define SHTTP_SERVER_HPP

#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <functional>
#include <httprequest.hpp>
#include <httpresponse.hpp>
#include <httpserverresult.hpp>
#include <iostream>
#include <thread>
#include <thread_pool.hpp>
#include <unordered_map>

namespace shttp {

constexpr size_t MAX_EVENTS = 10;

using file_descriptor_t = int;
using ResponseCallback = std::function<void(const HttpRequest&, HttpResponse&)>;
using Path = std::string;

namespace {
void setNonBlocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}
}  // namespace

// TCP connection
class HttpServer {
 public:
  inline static uint32_t BUFFER_SIZE = 4096;
  // HttpServer() = default;
  explicit HttpServer(uint16_t port,
                      uint16_t thread_count = std::max(
                          1u, std::thread::hardware_concurrency() - 1));

  HttpServer(const HttpServer&) = delete;
  HttpServer& operator=(const HttpServer&) = delete;
  HttpServer(HttpServer&&) = delete;
  HttpServer& operator=(HttpServer&&) = delete;

  ~HttpServer() {
    close(server_fd_);
    close(epoll_fd_);
  }

  void setRequestHandler(ResponseCallback cb) {
    request_handler_ = std::move(cb);
  }

  HttpServerResult run();

  void setStaticRoot(const Path& path) noexcept { static_root_ = path; }
  void setStaticRoot(Path&& path) noexcept { static_root_ = std::move(path); }

  uint16_t getPort() const noexcept { return port_; }

 private:
  ThreadPool thread_pool_;
  ResponseCallback request_handler_ = [](const HttpRequest& request,
                                         HttpResponse& response) {
    std::cout << request;
    response.setBody("Hello from HTTP Server\n");
  };
  Path static_root_;
  file_descriptor_t server_fd_;
  file_descriptor_t epoll_fd_;
  epoll_event ev{}, events[MAX_EVENTS];
  uint16_t port_ = 8080;

 private:
  HttpServerResult startServer(uint16_t port) noexcept;
  void configureEpoll();
  void acceptClientConnection();
  void proccesRequest(size_t event_index);
  void startEpollLoop();
  bool handleStaticFiles(const HttpRequest& request, HttpResponse& response);
};

}  // namespace shttp

#endif
