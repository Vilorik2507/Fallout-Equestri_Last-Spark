#pragma once
#include <memory>

#include "Item.h"

class Weapon : public Item, public std::enable_shared_from_this<Weapon> {
 public:
  Weapon(const std::string& name, float weight, int value, int damage,
         int dist);

  int getDamage() const { return damage; };
  int getDist() const { return dist; };
  void recalcDamage();
  void use(Player* user) override;

 private:
  int damage;
  int dist;
};
