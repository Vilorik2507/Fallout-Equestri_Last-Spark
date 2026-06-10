#ifndef ITEM_H
#define ITEM_H
#include <string>

class Player;

class Item {
public:
	Item(const std::string& name, int value);
	virtual ~Item() = default;

	const std::string& getName() const { return name_it; };
	int getValue() const { return value_it; };
	int getSellValue() const;
	virtual void use(Player* user);

protected:
	std::string name_it;
	int value_it;
};
#endif // !ITEM_H

