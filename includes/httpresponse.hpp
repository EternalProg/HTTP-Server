#ifndef SHTTP_RESPONSE_HPP
#define SHTTP_RESPONSE_HPP

#include <httpversion.hpp>
#include <unordered_map>

namespace shttp {
/*

Start Line
Headers
Empty Line
Body

*/

using Header = std::string;
using Content = std::string;
using Headers = std::unordered_map<Header, Content>;

class HttpStatusCode {
 public:
  enum Value : uint16_t {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,

    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,

    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,

    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    PAYLOAD_TOO_LARGE = 413,                // RFC7231
    URI_TOO_LONG = 414,                     // RFC7231
    UNSUPPORTED_MEDIA_TYPE = 415,           // RFC7231
    RANGE_NOT_SATISFIABLE = 416,            // RFC7233
    EXPECTATION_FAILED = 417,               // RFC7231
    IM_A_TEAPOT = 418,                      // RFC7168
    MISDIRECTED_REQUEST = 421,              // RFC7540
    UNPROCESSABLE_ENTITY = 422,             // RFC4918
    LOCKED = 423,                           // RFC4918
    FAILED_DEPENDENCY = 424,                // RFC4918
    TOO_EARLY = 425,                        // RFC8470
    UPGRADE_REQUIRED = 426,                 // RFC7231
    PRECONDITION_REQUIRED = 428,            // RFC6585
    TOO_MANY_REQUESTS = 429,                // RFC6585
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,  // RFC6585

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,

    UNKNOWN_STATUS
  };

  HttpStatusCode() = default;
  constexpr HttpStatusCode(Value HttpStatusCode) : value(HttpStatusCode) {}
  constexpr operator Value() const { return value; }
  // Prevent usage: if( HttpStatusCode)
  explicit operator bool() const = delete;
  static std::string toString(HttpStatusCode HttpStatusCode) noexcept;
  static HttpStatusCode fromString(const std::string& str_method);

 private:
  Value value;
};

class HttpResponse {
 public:
  HttpResponse() { setHeader("Content-Type", "text/plain"); }
  HttpResponse(HttpStatusCode status_code, Content body)
      : status_code_(status_code), body_(std::move(body)) {
    setHeader("Content-Length", std::to_string(body_.size()));
    setHeader("Content-Type", "text/plain");
  }

  void setHeader(const Header& header, const Content& content) noexcept {
    headers_[header] = content;
  }
  void setHeader(const Header& header, Content&& content) noexcept {
    headers_[header] = std::move(content);
  }

  void setStatus(HttpStatusCode status_code) noexcept {
    status_code_ = status_code;
  }

  void setBody(const Content& body) {
    body_ = body;
    setHeader("Content-Length", std::to_string(body_.size()));
  }

  void setBody(Content&& body) {
    body_ = std::move(body);
    setHeader("Content-Length", std::to_string(body_.size()));
  }

  void setVersion(HttpVersion version) { version_ = version; }

  std::string toString() const;

 private:
  HttpVersion version_ = HttpVersion::HTTP_1_1;
  HttpStatusCode status_code_ = HttpStatusCode::OK;
  Headers headers_;
  Content body_;
};

}  // namespace shttp

#endif