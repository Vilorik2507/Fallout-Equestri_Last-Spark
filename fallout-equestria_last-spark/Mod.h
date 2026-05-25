#pragma once
#include <map>
#include <string>

class Mod {
 public:
  Mod() = default;
  ~Mod() = default;

 private:
  std::string name_mod;
  std::map<std::string, int> stat_bonus;
};
