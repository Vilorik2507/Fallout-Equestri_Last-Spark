#pragma once
#include "CombatAI.h"
#include "Combatant.h"

class Enemy : public Combatant {
 public:
  Enemy(const std::string& name, int level, int hp, int damage, int initiative);
  ~Enemy() = default;

  void dropReward() {};
  void applyDamage(int damage) override { hp_en -= damage; };

  // Combatant interface
  int getHp() const override { return hp_en; };
  bool isAlive() const override { return hp_en <= 0; };
  int getInitiative() const override { return initiative_en; };

  const std::string& getName() const { return name_en; }
  int getDamage() const { return damage_en; }

 private:
  std::string name_en;
  int level_en;
  int hp_en;
  int max_hp_en;
  int damage_en;
  int initiative_en;
  CombatAI* ai_en;
};
