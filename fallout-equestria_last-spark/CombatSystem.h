#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "CombatAction.h"
#include "Combatant.h"
#include "Enemy.h"
#include "Player.h"

class CombatSystem {
 public:
  CombatSystem() = default;
  ~CombatSystem() = default;

  CombatSystem(const CombatSystem&) = delete;
  CombatSystem& operator=(const CombatSystem&) = delete;

  CombatSystem(CombatSystem&&) = default;
  CombatSystem& operator=(CombatSystem&&) = default;

  bool startCombat(Player& player,
                   std::vector<std::shared_ptr<Enemy>>& enemies);
  void executeAction(const CombatAction& action);
  void endCombat();

 private:
  std::vector<std::unique_ptr<Combatant>> participants;
  std::queue<std::unique_ptr<Combatant>> turn_order;
};
