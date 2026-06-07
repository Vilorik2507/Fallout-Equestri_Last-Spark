#pragma once
#include "Item.h"

class Consumable : public Item {
 public:
  Consumable(const std::string& name, float weight, int value, int heal);
  ~Consumable() = default;
  void use(Player* user) override;

 private:
  int heal;
};
