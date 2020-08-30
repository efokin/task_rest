
#include "Helper.hpp"
#include <iomanip>
#include <stdexcept>
#include <optional>
#include <algorithm>
#include <iterator>

std::string serializeTimePoint(const std::chrono::system_clock::time_point &time) {
  std::string result;
  std::time_t tt = std::chrono::system_clock::to_time_t(time);
  std::tm tm = *std::localtime(&tt);
  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  result = ss.str();
  return result;
}

std::optional<std::chrono::system_clock::time_point> deserializeTimePoint(const std::string &str) {
  std::tm timeinfo{};
  std::istringstream ss(str);
  ss >> std::get_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
  if (ss.fail()) {
    return {};
  }
  std::time_t tt = std::mktime(&timeinfo);
  return std::chrono::system_clock::from_time_t(tt);
}

std::string readStringFromIstream(std::istream& istr, size_t length) {
  std::string str(length, ' ');
  istr.read(&str[0], length);
  return str;
}