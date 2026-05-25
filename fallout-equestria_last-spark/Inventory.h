#pragma once
#include <vector>

#include "Item.h"
#include "Mod.h"
#include "Weapon.h"

class Inventory {
 public:
  Inventory() = default;
  ~Inventory() = default;

  void addItem(const Item& item);
  void removeItem(const Item& item);
  void craftMod(Weapon& weapon, const Mod& mod);

 private:
  std::vector<Item> items_list;
  float max_weight;
};
