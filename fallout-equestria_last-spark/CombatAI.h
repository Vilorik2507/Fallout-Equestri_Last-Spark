
#ifndef COMBAT_AI_H
#define COMBAT_AI_H
#include <memory>
#include <vector>

#include "CombatAction.h"
#include "Combatant.h"

class CombatAI {
 public:
  CombatAction chooseAction(
      Combatant* self, const std::vector<std::shared_ptr<Combatant>>& allies,
      const std::vector<std::shared_ptr<Combatant>>& enemies);
};
#endif  // !COMBAT_AI_H
