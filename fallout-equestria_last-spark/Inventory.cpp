#include "Inventory.h"

#include <iostream>

void Inventory::addItem(std::shared_ptr<Item> item) {
  items_list.push_back(item);
}
const std::vector<std::shared_ptr<Item>>& Inventory::getItems() const {
  return items_list;
}
void Inventory::removeItem(const std::shared_ptr<Item>& item) {
  auto it = std::find(items_list.begin(), items_list.end(), item);
  if (it != items_list.end()) {
    items_list.erase(it);
  }
}

void Inventory::listItems() const {
  if (items_list.empty()) {
    std::cout << "No items.\n";
    return;
  }
  for (size_t i = 0; i < items_list.size(); ++i) {
    std::cout << i + 1 << ". " << items_list[i]->getName()
              << " (value: " << items_list[i]->getValue() << ")\n";
  }
}
