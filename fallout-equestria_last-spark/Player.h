#pragma once
#include <map>
#include <string>
#include <vector>

#include "CharacterStats.h"
#include "Combatant.h"
#include "Effect.h"
#include "Faction.h"
#include "Inventory.h"
#include "Perk.h"
#include "Skill.h"
#include "Weapon.h"

class Player : public Combatant {
 public:
  Player() = default;
  ~Player() = default;

  void createNew();
  void levelUp();
  void addExp(int amount);
  void addPerk(const Perk& perk);
  void modifyReputation(const Faction& faction, int delta);
  int getDamage(const Weapon& weapon);
  void applyDamage(int damage) override;
  void healPlayer(int amount);
  void applyBuffEffect(const Effect& effect, int duration);

  // Combatant interface
  int getHp() const override;
  bool isAlive() const override;
  int getInitiative() const override;

 private:
  std::string name_pl;
  int level_pl;
  int exp_pl;
  CharacterStats stats_pl;
  std::vector<Skill> skills_pl;
  std::vector<Perk> perks_pl;
  Inventory inventory_pl;
  std::map<Faction, int> reputation_pl;
  int hp_pl;
  int max_hp_pl;
};
