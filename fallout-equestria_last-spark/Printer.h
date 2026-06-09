#pragma once
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

enum class SlowMode {
  CharByChar,  // посимвольно с задержкой между буквами
  LineByLine   // строка появляется целиком, затем пауза
};

class Printer {
 public:
  explicit Printer(int delay_ms = 50, SlowMode mode = SlowMode::LineByLine)
      : m_delay(delay_ms), m_mode(mode) {}

  void setMode(SlowMode mode) { m_mode = mode; }
  void setDelay(int ms) { m_delay = ms; }

  template <typename T>
  Printer& operator<<(const T& value) {
    std::stringstream ss;
    ss << value;
    outputString(ss.str());
    return *this;
  }

  Printer& operator<<(const char* str) {
    outputString(std::string(str));
    return *this;
  }

  Printer& operator<<(const std::string& str) {
    outputString(str);
    return *this;
  }

  Printer& operator<<(std::ostream& (*manip)(std::ostream&)) {
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

  static std::string replaceTags(
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

 private:
  void outputString(const std::string& str) {
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

  int m_delay;
  SlowMode m_mode;
};

extern Printer slow_cout;
