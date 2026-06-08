#include "Weapon.h"

#include "Player.h"
Weapon::Weapon(const std::string& name, float weight, int value, int damage,
               int dist)
    : Item(name, weight, value), damage(damage), dist(dist) {};

void Weapon::use(Player* user) { user->setActivWeopon(shared_from_this()); };
