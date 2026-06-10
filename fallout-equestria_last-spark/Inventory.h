#ifndef INVENTORY_H
#define INVENTORY_H
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
	void listItems() const;

private:
	std::vector<std::shared_ptr<Item>> items_list;
};
#endif // !INVENTORY_H

