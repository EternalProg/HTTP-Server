#include <http_response.hpp>
#include <ostream>

namespace shttp {

std::string HttpStatusCode::toString(HttpStatusCode HttpStatusCode) noexcept {
  switch (HttpStatusCode) {
    case CONTINUE:
      return "100 Continue";
    case SWITCHING_PROTOCOLS:
      return "101 Switching Protocols";
    case PROCESSING:
      return "102 Processing";

    case OK:
      return "200 OK";
    case CREATED:
      return "201 Created";
    case ACCEPTED:
      return "202 Accepted";
    case NON_AUTHORITATIVE_INFORMATION:
      return "203 Non-Authoritative Information";
    case NO_CONTENT:
      return "204 No Content";
    case RESET_CONTENT:
      return "205 Reset Content";
    case PARTIAL_CONTENT:
      return "206 Partial Content";

    case MULTIPLE_CHOICES:
      return "300 Multiple Choices";
    case MOVED_PERMANENTLY:
      return "301 Moved Permanently";
    case FOUND:
      return "302 Found";
    case SEE_OTHER:
      return "303 See Other";
    case NOT_MODIFIED:
      return "304 Not Modified";
    case USE_PROXY:
      return "305 Use Proxy";
    case TEMPORARY_REDIRECT:
      return "307 Temporary Redirect";

    case BAD_REQUEST:
      return "400 Bad Request";
    case UNAUTHORIZED:
      return "401 Unauthorized";
    case PAYMENT_REQUIRED:
      return "402 Payment Required";
    case FORBIDDEN:
      return "403 Forbidden";
    case NOT_FOUND:
      return "404 Not Found";
    case METHOD_NOT_ALLOWED:
      return "405 Method Not Allowed";
    case NOT_ACCEPTABLE:
      return "406 Not Acceptable";
    case PROXY_AUTHENTICATION_REQUIRED:
      return "407 Proxy Authentication Required";
    case REQUEST_TIMEOUT:
      return "408 Request Timeout";
    case CONFLICT:
      return "409 Conflict";
    case GONE:
      return "410 Gone";
    case LENGTH_REQUIRED:
      return "411 Length Required";
    case PRECONDITION_FAILED:
      return "412 Precondition Failed";
    case PAYLOAD_TOO_LARGE:                          // RFC7231
      return "413 Payload Too Large";                // RFC7231
    case URI_TOO_LONG:                               // RFC7231
      return "414 URI Too Long";                     // RFC7231
    case UNSUPPORTED_MEDIA_TYPE:                     // RFC7231
      return "415 Unsupported Media Type";           // RFC7231
    case RANGE_NOT_SATISFIABLE:                      // RFC7233
      return "416 Range Not Satisfiable";            // RFC7233
    case EXPECTATION_FAILED:                         // RFC7231
      return "417 Expectation Failed";               // RFC7231
    case IM_A_TEAPOT:                                // RFC7168
      return "418 I'm a teapot";                     // RFC7168
    case MISDIRECTED_REQUEST:                        // RFC7540
      return "421 Misdirected Request";              // RFC7540
    case UNPROCESSABLE_ENTITY:                       // RFC4918
      return "422 Unprocessable Entity";             // RFC4918
    case LOCKED:                                     // RFC4918
      return "423 Locked";                           // RFC4918
    case FAILED_DEPENDENCY:                          // RFC4918
      return "424 Failed Dependency";                // RFC4918
    case TOO_EARLY:                                  // RFC8470
      return "425 Too Early";                        // RFC8470
    case UPGRADE_REQUIRED:                           // RFC7231
      return "426 Upgrade Required";                 // RFC7231
    case PRECONDITION_REQUIRED:                      // RFC6585
      return "428 Precondition Required";            // RFC6585
    case TOO_MANY_REQUESTS:                          // RFC6585
      return "429 Too Many Requests";                // RFC6585
    case REQUEST_HEADER_FIELDS_TOO_LARGE:            // RFC6585
      return "431 Request Header Fields Too Large";  // RFC6585
    case INTERNAL_SERVER_ERROR:
      return "500 Internal Server Error";
    case NOT_IMPLEMENTED:
      return "501 Not Implemented";
    case BAD_GATEWAY:
      return "502 Bad Gateway";
    case SERVICE_UNAVAILABLE:
      return "503 Service Unavailable";
    case GATEWAY_TIMEOUT:
      return "504 Gateway Timeout";
    case HTTP_VERSION_NOT_SUPPORTED:
      return "505 HTTP Version Not Supported";
    default:
      return "Unknown Status";
  }
}

HttpStatusCode HttpStatusCode::fromString(const std::string& str_method) {
  if (str_method == "100 Continue") {
    return HttpStatusCode(CONTINUE);
  } else if (str_method == "101 Switching Protocols") {
    return HttpStatusCode(SWITCHING_PROTOCOLS);
  } else if (str_method == "102 Processing") {
    return HttpStatusCode(PROCESSING);
  } else if (str_method == "200 OK") {
    return HttpStatusCode(OK);
  } else if (str_method == "201 Created") {
    return HttpStatusCode(CREATED);
  } else if (str_method == "202 Accepted") {
    return HttpStatusCode(ACCEPTED);
  } else if (str_method == "203 Non-Authoritative Information") {
    return HttpStatusCode(NON_AUTHORITATIVE_INFORMATION);
  } else if (str_method == "204 No Content") {
    return HttpStatusCode(NO_CONTENT);
  } else if (str_method == "205 Reset Content") {
    return HttpStatusCode(RESET_CONTENT);
  } else if (str_method == "206 Partial Content") {
    return HttpStatusCode(PARTIAL_CONTENT);
  } else if (str_method == "300 Multiple Choices") {
    return HttpStatusCode(MULTIPLE_CHOICES);
  } else if (str_method == "301 Moved Permanently") {
    return HttpStatusCode(MOVED_PERMANENTLY);
  } else if (str_method == "302 Found") {
    return HttpStatusCode(FOUND);
  } else if (str_method == "303 See Other") {
    return HttpStatusCode(SEE_OTHER);
  } else if (str_method == "304 Not Modified") {
    return HttpStatusCode(NOT_MODIFIED);
  } else if (str_method == "305 Use Proxy") {
    return HttpStatusCode(USE_PROXY);
  } else if (str_method == "307 Temporary Redirect") {
    return HttpStatusCode(TEMPORARY_REDIRECT);
  } else if (str_method == "400 Bad Request") {
    return HttpStatusCode(BAD_REQUEST);
  } else if (str_method == "401 Unauthorized") {
    return HttpStatusCode(UNAUTHORIZED);
  } else if (str_method == "402 Payment Required") {
    return HttpStatusCode(PAYMENT_REQUIRED);
  } else if (str_method == "403 Forbidden") {
    return HttpStatusCode(FORBIDDEN);
  } else if (str_method == "404 Not Found") {
    return HttpStatusCode(NOT_FOUND);
  } else if (str_method == "405 Method Not Allowed") {
    return HttpStatusCode(METHOD_NOT_ALLOWED);
  } else if (str_method == "406 Not Acceptable") {
    return HttpStatusCode(NOT_ACCEPTABLE);
  } else if (str_method == "407 Proxy Authentication Required") {
    return HttpStatusCode(PROXY_AUTHENTICATION_REQUIRED);
  } else if (str_method == "408 Request Timeout") {
    return HttpStatusCode(REQUEST_TIMEOUT);
  } else if (str_method == "409 Conflict") {
    return HttpStatusCode(CONFLICT);
  } else if (str_method == "410 Gone") {
    return HttpStatusCode(GONE);
  } else if (str_method == "411 Length Required") {
    return HttpStatusCode(LENGTH_REQUIRED);
  } else if (str_method == "412 Precondition Failed") {
    return HttpStatusCode(PRECONDITION_FAILED);
  } else if (str_method == "413 Payload Too Large") {
    return HttpStatusCode(PAYLOAD_TOO_LARGE);
  } else if (str_method == "414 URI Too Long") {
    return HttpStatusCode(URI_TOO_LONG);
  } else if (str_method == "415 Unsupported Media Type") {
    return HttpStatusCode(UNSUPPORTED_MEDIA_TYPE);
  } else if (str_method == "416 Range Not Satisfiable") {
    return HttpStatusCode(RANGE_NOT_SATISFIABLE);
  } else if (str_method == "417 Expectation Failed") {
    return HttpStatusCode(EXPECTATION_FAILED);
  } else if (str_method == "418 I'm a teapot") {
    return HttpStatusCode(IM_A_TEAPOT);
  } else if (str_method == "421 Misdirected Request") {
    return HttpStatusCode(MISDIRECTED_REQUEST);
  } else if (str_method == "422 Unprocessable Entity") {
    return HttpStatusCode(UNPROCESSABLE_ENTITY);
  } else if (str_method == "423 Locked") {
    return HttpStatusCode(LOCKED);
  } else if (str_method == "424 Failed Dependency") {
    return HttpStatusCode(FAILED_DEPENDENCY);
  } else if (str_method == "425 Too Early") {
    return HttpStatusCode(TOO_EARLY);
  } else if (str_method == "426 Upgrade Required") {
    return HttpStatusCode(UPGRADE_REQUIRED);
  } else if (str_method == "428 Precondition Required") {
    return HttpStatusCode(PRECONDITION_REQUIRED);
  } else if (str_method == "429 Too Many Requests") {
    return HttpStatusCode(TOO_MANY_REQUESTS);
  } else if (str_method == "431 Request Header Fields Too Large") {
    return HttpStatusCode(REQUEST_HEADER_FIELDS_TOO_LARGE);
  } else if (str_method == "500 Internal Server Error") {
    return HttpStatusCode(INTERNAL_SERVER_ERROR);
  } else if (str_method == "501 Not Implemented") {
    return HttpStatusCode(NOT_IMPLEMENTED);
  } else if (str_method == "502 Bad Gateway") {
    return HttpStatusCode(BAD_GATEWAY);
  } else if (str_method == "503 Service Unavailable") {
    return HttpStatusCode(SERVICE_UNAVAILABLE);
  } else if (str_method == "504 Gateway Timeout") {
    return HttpStatusCode(GATEWAY_TIMEOUT);
  } else if (str_method == "505 HTTP Version Not Supported") {
    return HttpStatusCode(HTTP_VERSION_NOT_SUPPORTED);
  }
  return HttpStatusCode(UNKNOWN_STATUS);
}
std::string HttpResponse::toString() const {
  std::string result;

  result += HttpVersion::toString(version_) + " " +
            HttpStatusCode::toString(status_code_) + "\r\n";

  for (auto& [header, content] : headers_) {
    result += header + ": " + content + "\r\n";
  }

  result += "\r\n";

  result += body_;

  return result;
}

std::ostream& operator<<(std::ostream& out, const HttpResponse& res) {
  out << res.toString();
  return out;
}

}  // namespace shttp