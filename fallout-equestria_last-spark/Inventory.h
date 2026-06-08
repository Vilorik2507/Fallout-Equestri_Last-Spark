#pragma once
#include <memory>
#include <vector>

#include "Item.h"
#include "Mod.h"
#include "Weapon.h"

class Inventory {
 public:
  Inventory() = default;
  ~Inventory() = default;

  const std::vector<std::shared_ptr<Item>>& getItems() const;
  void addItem(std::shared_ptr<Item> item);
  void removeItem(const std::shared_ptr<Item>& item);
  void craftMod(Weapon& weapon, const Mod& mod);
  void listItems() const;

 private:
  std::vector<std::shared_ptr<Item>> items_list;
  float max_weight;
};
