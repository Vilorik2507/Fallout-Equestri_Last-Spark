#ifndef PRINTER_H
#define PRINTER_H
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
  Printer& operator<<(const T& value);

  Printer& operator<<(const char* str);

  Printer& operator<<(const std::string& str);

  Printer& operator<<(std::ostream& (*manip)(std::ostream&));

  static std::string replaceTags(
      const std::string& str,
      const std::unordered_map<std::string, std::string>& tags);

 private:
  void outputString(const std::string& str);

  int m_delay;
  SlowMode m_mode;
};

extern Printer slow_cout;

#endif  // !PRINTER_H
