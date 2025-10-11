#include <cstdint>
#include <http_version.hpp>
#include <string>

namespace shttp {
std::string HttpVersion::toString(HttpVersion HttpVersion) {
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

HttpVersion HttpVersion::fromString(const std::string& str_version) {
  if (str_version == "HTTP/1.0") {
    return HttpVersion(HTTP_1_0);
  } else if (str_version == "HTTP/1.1") {
    return HttpVersion(HTTP_1_1);
  } else if (str_version == "HTTP/2.0") {
    return HttpVersion(HTTP_2_0);
  } else if (str_version == "HTTP/3.0") {
    return HttpVersion(HTTP_3_0);
  }
  return HttpVersion(UNKNOWN);
}
}  // namespace shttp