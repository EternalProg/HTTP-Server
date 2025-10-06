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
    UNKNOWN // For unrecognized methods
  };

  HttpMethod() = default;
  constexpr HttpMethod(Value httpMethod) : value(httpMethod) {}

  constexpr operator Value() const { return value; }

  // Prevent usage: if(httpMethod)
  explicit operator bool() const = delete;

  static std::string toString(HttpMethod HttpMethod);

  static HttpMethod fromString(const std::string &str_method);

private:
  Value value;
};

struct HttpMethodHash {
  size_t operator()(HttpMethod method) const noexcept;
};

} // namespace shttp
#endif
