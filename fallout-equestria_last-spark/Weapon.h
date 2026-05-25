#pragma once
#include "Item.h"

class Weapon : public Item {
 public:
  Weapon() = default;
  ~Weapon() = default;

  void recalcDamage();

 private:
  // Поля по вашему усмотрению
};
