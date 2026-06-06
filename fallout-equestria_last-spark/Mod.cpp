#include "Mod.h"
Mod::Mod(std::map<std::string, int>& stats) : stat_bonus(stats) {}
const std::map<std::string, int>& Mod::getBonuses() const { return stat_bonus; }
const std::string& Mod::getName() const { return name_mod; }
