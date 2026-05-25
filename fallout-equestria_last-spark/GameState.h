#pragma once
#include <map>
#include <string>
#include <vector>

#include "Faction.h"
#include "Location.h"
#include "Player.h"
#include "Quest.h"

class GameState {
 public:
  GameState() = default;
  ~GameState() = default;

  std::string serializeState(Player& player, std::vector<Quest*>& quests,
                             std::map<std::string, Location*>& locations,
                             std::map<Faction, int>& reputation);
  GameState* deserializeState(const std::string& data);

 private:
  // Поля по вашему усмотрению
};
