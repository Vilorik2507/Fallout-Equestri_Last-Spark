#pragma once
#include <string>

#include "Dialogue.h"
#include "Faction.h"

class NPC {
 public:
  NPC() = default;
  ~NPC() = default;

  void talkWithPlayer();

 private:
  std::string name_npc;
  Dialogue* dialogue;
  Faction* faction_npc;
};
