#pragma once
#include <map>
#include <string>

#include "Combatant.h"

class Effect {
 public:
  Effect() = default;
  ~Effect() = default;

  void apply(Combatant* target);
  void tick();

 private:
  std::string name;
  int duration;
  std::map<std::string, int> modifiers;
};
