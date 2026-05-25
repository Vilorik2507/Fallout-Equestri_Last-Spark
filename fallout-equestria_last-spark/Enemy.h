#pragma once
#include "CombatAI.h"
#include "Combatant.h"

class Enemy : public Combatant {
 public:
  Enemy() = default;
  ~Enemy() = default;

  void dropReward();
  void applyDamage(int damage) override;

  // Combatant interface
  int getHp() const override;
  bool isAlive() const override;
  int getInitiative() const override;

 private:
  int level_en;
  int hp_en;
  CombatAI* ai_en;
};
