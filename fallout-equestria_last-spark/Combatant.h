#pragma once
class Combatant {
 public:
  virtual ~Combatant() = default;

  virtual int getHp() const = 0;
  virtual void applyDamage(int damage) = 0;
  virtual bool isAlive() const = 0;
  virtual int getInitiative() const = 0;
};
