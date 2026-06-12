#include "Printer.h"

Printer slow_cout(200, SlowMode::LineByLine);

template <typename T>
Printer& Printer::operator<<(const T& value) {
  std::stringstream ss;
  ss << value;
  outputString(ss.str());
  return *this;
}

Printer& Printer::operator<<(const char* str) {
  outputString(std::string(str));
  return *this;
}

Printer& Printer::operator<<(const std::string& str) {
  outputString(str);
  return *this;
}

Printer& Printer::operator<<(std::ostream& (*manip)(std::ostream&)) {
  if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
    std::cout << std::endl;
    if (m_mode == SlowMode::LineByLine) {
      std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
    }
  } else if (manip ==
             static_cast<std::ostream& (*)(std::ostream&)>(std::flush)) {
    std::cout << std::flush;
  } else {
    std::cout << manip;
  }
  return *this;
}

std::string Printer::replaceTags(
    const std::string& str,
    const std::unordered_map<std::string, std::string>& tags) {
  std::string result;
  result.reserve(str.size());

  for (size_t i = 0; i < str.size();) {
    if (str[i] == '{') {
      size_t end = str.find('}', i);
      if (end != std::string::npos) {
        std::string key = str.substr(i, end - i + 1);
        auto it = tags.find(key);
        if (it != tags.end()) {
          result += it->second;
          i = end + 1;
          continue;
        }
      }
    }
    result += str[i];
    ++i;
  }
  return result;
}

void Printer::outputString(const std::string& str) {
  if (m_mode == SlowMode::CharByChar) {
    for (char c : str) {
      std::cout << c << std::flush;
      if (c != '\n') {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
      }
    }
  } else {
    std::cout << str << std::flush;
    if (!str.empty() && str.back() == '\n') {
      std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
    }
  }
}
