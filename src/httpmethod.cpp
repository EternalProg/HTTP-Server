#include <httpmethod.hpp>
#include <string>

namespace shttp {
std::string HttpMethod::toString(HttpMethod HttpMethod) {
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

HttpMethod HttpMethod::fromString(const std::string& str_method) {
  if (str_method == "GET") {
    return HttpMethod(GET);
  } else if (str_method == "POST") {
    return HttpMethod(POST);
  } else if (str_method == "PUT") {
    return HttpMethod(PUT);
  } else if (str_method == "DELETE") {
    return HttpMethod(DELETE);
  } else if (str_method == "HEAD") {
    return HttpMethod(HEAD);
  } else if (str_method == "OPTIONS") {
    return HttpMethod(OPTIONS);
  } else if (str_method == "PATCH") {
    return HttpMethod(PATCH);
  } else if (str_method == "CONNECT") {
    return HttpMethod(CONNECT);
  } else if (str_method == "TRACE") {
    return HttpMethod(TRACE);
  }
  return HttpMethod(UNKNOWN);
}

}  // namespace shttp