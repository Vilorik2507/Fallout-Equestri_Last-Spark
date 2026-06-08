#pragma once
#include "Item.h"

class Armor : public Item {
 public:
  Armor(const std::string& name, float weight, int value, int defense);
  ~Armor() = default;

 private:
  int defense;
};
