#ifndef SHTTP_REQUEST_HPP
#define SHTTP_REQUEST_HPP

#include <http_method.hpp>
#include <http_version.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace shttp {

using Uri = std::string;
using Header = std::string;
using Content = std::string;
using Headers = std::unordered_map<Header, Content>;

class HttpRequest {
public:
  HttpRequest() = default;

  HttpRequest(const std::string &raw_request);
  HttpRequest(char *buffer, std::size_t len)
      : HttpRequest(std::string(buffer, len)) {}

  void parse(const std::string &raw_request);

  friend std::ostream &operator<<(std::ostream &out,
                                  const HttpRequest request) {
    out << request.toString();
    return out;
  }

  constexpr void setMethod(const HttpMethod &method) { method_ = method; }
  constexpr void setUri(const Uri &uri) { uri_ = uri; }
  constexpr void setVersion(const HttpVersion &version) { version_ = version; }
  void setHeader(const Header &key, const Content &value) {
    headers_[key] = value;
  }
  constexpr void setBody(const std::string &body) { body_ = body; }

  [[nodiscard]] const HttpMethod &getMethod() const { return method_; }
  [[nodiscard]] const Uri &getUri() const { return uri_; }
  [[nodiscard]] const Uri &getPath() const { return uri_; }
  [[nodiscard]] const HttpVersion &getVersion() const { return version_; }
  [[nodiscard]] const Headers &getHeaders() const { return headers_; }
  [[nodiscard]] const Content &getBody() const { return body_; }

  [[nodiscard]] const Content &getHeaderContent(const Header &header) {
    return headers_[header];
  }

  [[nodiscard]] bool hasHeader(const Header &header) noexcept {
    return headers_.find(header) != headers_.end();
  }

  [[nodiscard]] std::string toString() const noexcept;

private:
  HttpMethod method_;   // e.g., "GET", "POST"
  Uri uri_;             // e.g., "/index.html"
  HttpVersion version_; // e.g., "HTTP/1.1"
  Headers headers_;
  Content body_; // For POST/PUT requests
};

} // namespace shttp

#endif
