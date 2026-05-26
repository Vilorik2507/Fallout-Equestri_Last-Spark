#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "Armor.h"
#include "Consumable.h"
#include "Dialogue.h"
#include "Enemy.h"
#include "Faction.h"
#include "Item.h"
#include "Location.h"
#include "NPC.h"
#include "Quest.h"
#include "Weapon.h"

class ResourceManager {
 public:
  ResourceManager() = delete;
  ~ResourceManager() = delete;

  static void init(const std::string& resourcePath);

  static Dialogue* getDialogue(const std::string& id);
  static NPC* getNPC(const std::string& id);
  static Item* getItem(const std::string& id);
  static Location* getLocation(const std::string& id);
  static Faction* getFaction(const std::string& id);
  static Enemy* getEnemy(const std::string& id);
  static Quest* getQuest(const std::string& id);

  static void cleanup();

 private:
  static std::unordered_map<std::string, std::unique_ptr<Dialogue>> s_dialogues;
  static std::unordered_map<std::string, std::unique_ptr<NPC>> s_npcs;
  static std::unordered_map<std::string, std::unique_ptr<Item>> s_items;
  static std::unordered_map<std::string, std::unique_ptr<Location>> s_locations;
  static std::unordered_map<std::string, std::unique_ptr<Faction>> s_factions;
  static std::unordered_map<std::string, std::unique_ptr<Enemy>> s_enemies;
  static std::unordered_map<std::string, std::unique_ptr<Quest>> s_quests;

  static void loadDialogues(const std::string& filePath);
  static void loadNPCs(const std::string& filePath);
  static void loadItems(const std::string& filePath);
  static void loadLocations(const std::string& filePath);
  static void loadFactions(const std::string& filePath);
  static void loadEnemies(const std::string& filePath);
  static void loadQuests(const std::string& filePath);
};
