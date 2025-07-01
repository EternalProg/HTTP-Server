#include <httprequest.hpp>
#include <httpversion.hpp>
#include <sstream>
#include <utils.hpp>

using shttp::HttpRequest;
using shttp::utils::copy_trim;

// Maybe should rewrite it using string_view
HttpRequest::HttpRequest(const std::string& raw_request) {
  // parse http
  std::istringstream isstream(raw_request);
  std::string line;

  // Parse Request line
  // method - uri - version
  std::getline(isstream, line);
  std::istringstream ss(line);
  std::string method, uri, version;
  ss >> method >> uri >> version;

  method_ = HttpMethod::fromString(method);
  uri_ = uri;
  version_ = HttpVersion::fromString(version);

  // Headers (key-value pair)
  while (getline(isstream, line)) {
    size_t colon = line.find(':');
    if (colon != std::string::npos && line != "\r") {
      Header header = line.substr(0, colon);
      Content content = line.substr(colon + 1);
      headers_[copy_trim(header)] = copy_trim(content);
    }
  }

  // body
  body_ = std::string((std::istreambuf_iterator<char>(isstream)), {});
}

std::string HttpRequest::toString() const noexcept {
  std::string result = HttpMethod::toString(method_) + " " + uri_ + " " +
                       HttpVersion::toString(version_) + "\n";
  for (const auto& [header, content] : headers_) {
    result += header + ": " + content + "\n";
  }
  result += body_ + "\n";
  return result;
}