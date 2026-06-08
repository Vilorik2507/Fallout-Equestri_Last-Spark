#include "Effect.h"

Effect::Effect(const std::string& name, int duration,
               const std::map<std::string, int>& modifiers)
    : name(name), duration(duration), modifiers(modifiers) {}

Effect::~Effect() {}

void Effect::apply(Combatant* target) const {
  if (modifiers.find("damage") != modifiers.end()) {
    target->applyDamage(modifiers.at("damage"));
  }
}

void Effect::tick(Combatant* target) {
  duration--;
  if (duration <= 0) return;
  apply(target);
}
