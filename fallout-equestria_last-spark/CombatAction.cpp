#include "CombatAction.h"

#include <cstdlib>
#include <iostream>

#include "Printer.h"

bool CombatAction::calculateHit() const { return (rand() % 100) < 80; }

void CombatAction::addEffect(const Effect& effect) {
  effects.push_back(effect);
}
CombatAction::CombatAction(Combatant* source, Combatant* target,
                           const std::string& type)
    : source(source), target(target), type(type) {}

void CombatAction::apply() const {
  if (type == "attack") {
    int damage = source->getDamage();
    target->applyDamage(damage);
    slow_cout << source->getName() << " атакует " << target->getName()
              << " и наносит " << damage << " урона!" << std::endl;
  }
  for (const auto& effect : effects) {
    effect.apply(target);
  }
}
