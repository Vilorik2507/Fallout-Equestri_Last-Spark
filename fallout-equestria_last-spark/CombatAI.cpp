#include "CombatAI.h"

#include <cstdlib>

#include "Enemy.h"
CombatAction CombatAI::chooseAction(
    Combatant* self, const std::vector<std::shared_ptr<Combatant>>& allies,
    const std::vector<std::shared_ptr<Combatant>>& enemies) {
  // »щем живого врага, не равного self
  Combatant* target = nullptr;
  for (const auto& e : enemies) {
    if (e.get() != self && e->isAlive()) {
      target = e.get();
      break;
    }
  }
  if (!target) {
    // fallback Ц возможно, self Ц это игрок, а enemies Ц враги
    for (const auto& e : enemies) {
      if (e->isAlive()) {
        target = e.get();
        break;
      }
    }
  }
  if (!target) target = self;  // не должно случитьс€

  // ƒвижение, если рассто€ние > 2
  if (auto* enemy = dynamic_cast<Enemy*>(self)) {
    if (enemy->getDistance() > 2) {
      return CombatAction(self, target, "move");
    }
  }
  return CombatAction(self, target, "attack");
}
