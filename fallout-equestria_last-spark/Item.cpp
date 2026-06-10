#include "Item.h"
Item::Item(const std::string& name, int value)
    : name_it(name), value_it(value) {};
int Item::getSellValue() const { return value_it / 2; }
void Item::use(Player* user) {}
