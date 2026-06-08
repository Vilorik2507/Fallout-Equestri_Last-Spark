#pragma once
#include <string>

class Skill {
 public:
  Skill() = default;
  ~Skill() = default;

 private:
  std::string name_sk;
  int value_sk;
};
