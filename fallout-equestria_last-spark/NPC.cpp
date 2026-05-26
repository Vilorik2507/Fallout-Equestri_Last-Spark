#include "NPC.h"
NPC::NPC(const std::string& name, Dialogue* dialogue, Faction* faction)
    : name_npc(name), dialogue(dialogue), faction_npc(faction) {};
