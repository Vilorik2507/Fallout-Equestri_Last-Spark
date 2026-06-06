#include "Item.h"
Item::Item(const std::string& name, float weight, int value)
    : name_it(name), weight_it(weight), value_it(value) {};
int Item::getSellValue() const { return value_it / 2; }
