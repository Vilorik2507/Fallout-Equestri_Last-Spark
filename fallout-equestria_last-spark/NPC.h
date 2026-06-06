#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Dialogue.h"
#include "Faction.h"

class Item;

class NPC {
 public:
  NPC(const std::string& name, const Dialogue* dialogue,
      const Faction* faction);
  ~NPC() = default;

  void talkWithPlayer();
  const std::string& getName() const { return name_npc; };
  const Dialogue* getDialogue() const { return dialogue; };
  const Faction* getFaction() const { return faction_npc; };
  void addShopItem(std::shared_ptr<Item> item);
  bool hasShop() const;
  void trade();

 private:
  std::string name_npc;
  const Dialogue* dialogue;
  const Faction* faction_npc;
  std::vector<std::shared_ptr<Item>> shop_items_npc;
};
