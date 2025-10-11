#ifndef SHTTP_SERVER_RESULT_HPP
#define SHTTP_SERVER_RESULT_HPP

#include <string>

namespace shttp {
enum class HttpServerStatus {
  OK,
  SOCKET_ERROR,
  BIND_ERROR,
  LISTEN_ERROR,
  FILE_NOT_FOUND,
  ERROR
};

struct HttpServerResult {
  HttpServerStatus status;
  std::string status_message;
};

}  // namespace shttp

#endif