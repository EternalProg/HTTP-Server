#ifndef SHTTP_VERSION_HPP
#define SHTTP_VERSION_HPP

#include <cstdint>
#include <string>

namespace shttp {

class HttpVersion {
 public:
  enum Value : uint8_t {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0,
    HTTP_3_0,
    UNKNOWN  // For unrecognized versions
  };

  HttpVersion() = default;
  constexpr HttpVersion(Value HttpVersion) : value(HttpVersion) {}

  constexpr operator Value() const { return value; }

  // Prevent usage: if(httpMethod)
  explicit operator bool() const = delete;

  static std::string to_string(HttpVersion HttpVersion) {
    switch (HttpVersion) {
      case HTTP_1_0:
        return "HTTP/1.0";
      case HTTP_1_1:
        return "HTTP/1.1";
      case HTTP_2_0:
        return "HTTP/2.0";
      case HTTP_3_0:
        return "HTTP/3.0";
      default:
        return "UNKNOWN";
    }
  }

 private:
  Value value;
};

}  // namespace shttp
#endif