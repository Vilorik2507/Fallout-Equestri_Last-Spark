#pragma once

#include <iostream>

#include "CombatAI.h"
#include "Combatant.h"

class Enemy : public Combatant {
 public:
  Enemy(const std::string& name, int level, int hp, int damage, int initiative,
        const std::string& lootTableId);

  void dropReward();
  void applyDamage(int damage) override;

  // Combatant interface
  int getHp() const override;
  bool isAlive() const override;
  int getInitiative() const override;
  const std::string& getName() const override;
  int getDamage() override;

  int getLevel() const;
  std::string& getLootTableId();
  void setDistance(int distance);
  int getDistance() const;

 private:
  std::string loot_table_en;
  std::string name_en;
  int level_en;
  int hp_en;
  int max_hp_en;
  int damage_en;
  int initiative_en;
  CombatAI* ai_en;
  int distance_en;
};
