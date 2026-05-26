#include "Armor.h"
Armor::Armor(const std::string& name, float weight, int value, int defense)
    : Item(name, weight, value), defense(defense) {};
