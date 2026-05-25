#pragma once
#include <string>

class Item {
 public:
  virtual ~Item() = default;

 private:
  std::string name_it;
  float weight_it;
  int value_it;
};
