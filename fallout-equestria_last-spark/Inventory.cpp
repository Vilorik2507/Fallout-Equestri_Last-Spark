#include "Inventory.h"
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
