#include <algorithm>
#include <cctype>
#include <http_method.hpp>
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

HttpMethod HttpMethod::fromString(const std::string &str_method) {
  std::string method = str_method;
  std::transform(method.begin(), method.end(), method.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  if (method == "GET") {
    return HttpMethod(GET);
  } else if (method == "POST") {
    return HttpMethod(POST);
  } else if (method == "PUT") {
    return HttpMethod(PUT);
  } else if (method == "DELETE") {
    return HttpMethod(DELETE);
  } else if (method == "HEAD") {
    return HttpMethod(HEAD);
  } else if (method == "OPTIONS") {
    return HttpMethod(OPTIONS);
  } else if (method == "PATCH") {
    return HttpMethod(PATCH);
  } else if (method == "CONNECT") {
    return HttpMethod(CONNECT);
  } else if (method == "TRACE") {
    return HttpMethod(TRACE);
  }
  return HttpMethod(UNKNOWN);
}

size_t HttpMethodHash::operator()(HttpMethod method) const noexcept {
  return static_cast<size_t>(method);
}

} // namespace shttp
