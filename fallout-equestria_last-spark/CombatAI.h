#pragma once
#include <vector>

#include "CombatAction.h"
#include "Combatant.h"

class CombatAI {
 public:
  CombatAI() = default;
  ~CombatAI() = default;

  CombatAction* chooseAction(Combatant* enemy, std::vector<Combatant*>& allies,
                             std::vector<Combatant*>& enemies);

 private:
  // Поля по вашему усмотрению
};
