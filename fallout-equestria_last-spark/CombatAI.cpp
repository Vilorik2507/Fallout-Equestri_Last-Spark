#include "CombatAI.h"

#include <cstdlib>

#include "Enemy.h"
CombatAction CombatAI::chooseAction(
    Combatant* self, const std::vector<std::shared_ptr<Combatant>>& allies,
    const std::vector<std::shared_ptr<Combatant>>& enemies) {
  Combatant* target = nullptr;
  for (const auto& e : enemies) {
    if (e && e->isAlive()) {
      target = e.get();
      break;
    }
  }
  if (!target) {
    for (const auto& e : enemies) {
      if (e->isAlive()) {
        target = e.get();
        break;
      }
    }
  }
  if (!target) target = self;

  if (auto* enemy = dynamic_cast<Enemy*>(self)) {
    if (enemy->getDistance() > 2) {
      return CombatAction(self, target, "move");
    }
  }
  return CombatAction(self, target, "attack");
}
