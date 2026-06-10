#ifndef CONSUMABLE_H
#define CONSUMABLE_H
#include "Item.h"

class Consumable : public Item {
public:
	Consumable(const std::string& name, int value, int heal);
	~Consumable() = default;
	void use(Player* user) override;

private:
	int heal;
};
#endif // !CONSUMABLE_H

