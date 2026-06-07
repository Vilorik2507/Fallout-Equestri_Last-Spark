#include "CombatAction.h"

#include <cstdlib>
#include <iostream>

CombatAction::CombatAction(Combatant* source, Combatant* target,
                           const std::string& type)
    : source(source), target(target), type(type) {}

bool CombatAction::calculateHit() const { return (rand() % 100) < 80; }

void CombatAction::apply() const {
  if (type == "attack") {
    int damage = 10;  // В реальности из характеристик
    target->applyDamage(damage);
    std::cout << source->getName() << " hits " << target->getName() << " for "
              << damage << " damage!" << std::endl;
  }
  for (auto& effect : effects) {
    effect.apply(target);
  }
}

void CombatAction::addEffect(const Effect& effect) {
  effects.push_back(effect);
}
