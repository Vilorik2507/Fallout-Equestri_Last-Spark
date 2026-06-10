#ifndef ARMOR_H
#define ARMOR_H
#include "Item.h"

class Armor : public Item {
public:
	Armor(const std::string& name, int value, int defense);
	~Armor() = default;

private:
	int defense;
};
#endif // !ARMOR_H


