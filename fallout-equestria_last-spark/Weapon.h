#pragma once
#include "Item.h"

class Weapon : public Item {
 public:
  Weapon(const std::string& name, float weight, int value, int damage);
  ~Weapon() = default;

  int getDamage() const { return damage; };
  void recalcDamage();

 private:
  int damage;
};
