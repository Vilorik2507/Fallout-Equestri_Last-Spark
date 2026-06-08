#pragma once
#include <string>

class CharacterStats {
 public:
  CharacterStats() = default;
  ~CharacterStats() = default;

  int getModifierStat(const std::string& stat) const;

 private:
  int strength_ch;
  int perception_ch;
  int endurance_ch;
  int charisma_ch;
  int intelligence_ch;
  int luck_ch;
};
