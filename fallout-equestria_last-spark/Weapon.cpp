#include "Weapon.h"
Weapon::Weapon(const std::string& name, float weight, int value, int damage)
    : Item(name, weight, value), damage(damage) {};
