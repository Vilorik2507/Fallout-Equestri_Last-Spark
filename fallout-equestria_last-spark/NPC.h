#pragma once
#include <string>

#include "Dialogue.h"
#include "Faction.h"

class NPC {
 public:
  NPC(const std::string& name, Dialogue* dialogue, Faction* faction);
  ~NPC() = default;

  void talkWithPlayer();
  const std::string& getName() const { return name_npc; };

 private:
  std::string name_npc;
  Dialogue* dialogue;
  Faction* faction_npc;
};
