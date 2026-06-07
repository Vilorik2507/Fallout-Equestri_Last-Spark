#include "Consumable.h"

#include "Player.h"

Consumable::Consumable(const std::string& name, float weight, int value,
                       int heal)
    : Item(name, weight, value), heal(heal) {};
void Consumable::use(Player* user) { user->healPlayer(heal); }
