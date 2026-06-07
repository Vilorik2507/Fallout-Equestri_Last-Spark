#pragma once
#include <string>

class Combatant {
 public:
  virtual ~Combatant() = default;

  virtual int getHp() const = 0;
  virtual void applyDamage(int damage) = 0;
  virtual bool isAlive() const = 0;
  virtual int getInitiative() const = 0;
  virtual const std::string& getName() const = 0;
};
