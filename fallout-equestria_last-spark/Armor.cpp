#include "Armor.h"
Armor::Armor(const std::string& name, int value, int defense)
    : Item(name, value), defense(defense) {};
