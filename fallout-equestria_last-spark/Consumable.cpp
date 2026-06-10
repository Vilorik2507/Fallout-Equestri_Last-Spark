#include "Consumable.h"

#include "Player.h"
void Consumable::use(Player* user) { user->healPlayer(heal); }
Consumable::Consumable(const std::string& name, int value,
                       int heal)
    : Item(name, value), heal(heal) {};