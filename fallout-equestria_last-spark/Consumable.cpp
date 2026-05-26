#include "Consumable.h"
Consumable::Consumable(const std::string& name, float weight, int value,
                       int heal)
    : Item(name, weight, value), heal(heal) {};
