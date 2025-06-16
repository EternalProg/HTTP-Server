#ifndef SHTTP_METHOD_HPP
#define SHTTP_METHOD_HPP

#include <cstdint>
#include <string>
namespace shttp {

class HttpMethod {
 public:
  enum Value : uint8_t {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    PATCH,
    CONNECT,
    TRACE,
    UNKNOWN  // For unrecognized methods
  };

  HttpMethod() = default;
  constexpr HttpMethod(Value httpMethod) : value(httpMethod) {}

  constexpr operator Value() const { return value; }

  // Prevent usage: if(httpMethod)
  explicit operator bool() const = delete;

  static std::string to_string(HttpMethod HttpMethod) {
    switch (HttpMethod) {
      case GET:
        return "GET";
      case POST:
        return "POST";
      case PUT:
        return "PUT";
      case DELETE:
        return "DELETE";
      case HEAD:
        return "HEAD";
      case OPTIONS:
        return "OPTIONS";
      case PATCH:
        return "PATCH";
      case CONNECT:
        return "CONNECT";
      case TRACE:
        return "TRACE";
      default:
        return "UNKNOWN";
    }
  }

 private:
  Value value;
};

}  // namespace shttp
#endif