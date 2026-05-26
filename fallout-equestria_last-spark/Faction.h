#pragma once
#include <string>

class Faction {
 public:
  Faction(int id, const std::string& name);
  ~Faction() = default;

 private:
  int id_fac;
  std::string name_fac;
};
