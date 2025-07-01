#ifndef SHTTP_UTILS_HPP
#define SHTTP_UTILS_HPP

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <httpserverresult.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <utils.hpp>
#include <vector>
namespace shttp::utils {

// trim from start (in place)
inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

inline void trim(std::string &str) {
  ltrim(str);
  rtrim(str);
}

inline std::string copy_trim(const std::string &str) {
  std::string copy_str(str);
  ltrim(copy_str);
  rtrim(copy_str);
  return copy_str;
}

inline std::string getMimeType(const std::string &path);
inline std::optional<std::string> readFromFile(const std::string &path);
inline std::string sanitizePath(const std::string &path);

std::optional<std::string> readFromFile(const std::string &path) {
  namespace fs = std::filesystem;

  if (!fs::exists(path) || !fs::is_regular_file(path)) {
    return std::nullopt;
  }

  std::ifstream ifs(path);
  if (!ifs) {
    return std::nullopt;
  }
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  return content;
}

std::string getMimeType(const std::string &path) {
  if (path.ends_with(".html")) return "text/html";
  if (path.ends_with(".css")) return "text/css";
  if (path.ends_with(".js")) return "application/javascript";
  if (path.ends_with(".png")) return "image/png";
  if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
  return "application/octet-stream";
}

std::string sanitizePath(const std::string &path) {
  if (path.empty()) return "/";

  std::vector<std::string> parts;
  std::istringstream stream(path);
  std::string segment;

  while (std::getline(stream, segment, '/')) {
    if (segment.empty() || segment == ".") {
      // skip empty and '.' segments
      continue;
    }
    if (segment == "..") {
      // Go up one directory (pop last if possible)
      if (!parts.empty()) {
        parts.pop_back();
      }
      // else ignore to prevent going above root
    } else {
      parts.push_back(segment);
    }
  }

  // Rebuild sanitized path
  std::string sanitized = "/";
  for (size_t i = 0; i < parts.size(); ++i) {
    sanitized += parts[i];
    if (i + 1 < parts.size()) sanitized += "/";
  }

  return sanitized;
}
}  // namespace shttp::utils

#endif