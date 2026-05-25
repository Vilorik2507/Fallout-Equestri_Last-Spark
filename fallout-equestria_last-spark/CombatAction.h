#pragma once
#include <string>
#include <vector>

#include "Combatant.h"
#include "Effect.h"

class CombatAction {
 public:
  CombatAction() = default;
  ~CombatAction() = default;

  bool calculateHit();
  void apply();

 private:
  Combatant* source;
  Combatant* target;
  std::string type;
  std::vector<Effect> effects;
};
