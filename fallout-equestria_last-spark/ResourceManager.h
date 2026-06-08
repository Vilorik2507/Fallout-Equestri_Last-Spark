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
#include "Mod.h"
#include "NPC.h"
#include "Quest.h"
#include "Weapon.h"

struct LootEntry {
  std::string itemId;
  float chance;  // 0.0 - 1.0
};

class ResourceManager {
 public:
  ResourceManager() = delete;
  ~ResourceManager() = delete;

  static void init(const std::string& resourcePath);

  static std::shared_ptr<Dialogue> getDialogue(const std::string& id);
  static std::shared_ptr<NPC> getNPC(const std::string& id);
  static std::shared_ptr<Item> getItem(const std::string& id);
  static std::shared_ptr<Location> getLocation(const std::string& id);
  static std::shared_ptr<Faction> getFaction(const std::string& id);
  static std::shared_ptr<Enemy> getEnemy(const std::string& id);
  static std::shared_ptr<Quest> getQuest(const std::string& id);
  static std::shared_ptr<Mod> getMod(const std::string& id);
  static void generateLoot(const std::string& enemyId);

  static void cleanup();

 private:
  static std::unordered_map<std::string, std::shared_ptr<Dialogue>> s_dialogues;
  static std::unordered_map<std::string, std::shared_ptr<NPC>> s_npcs;
  static std::unordered_map<std::string, std::shared_ptr<Item>> s_items;
  static std::unordered_map<std::string, std::shared_ptr<Location>> s_locations;
  static std::unordered_map<std::string, std::shared_ptr<Faction>> s_factions;
  static std::unordered_map<std::string, std::shared_ptr<Enemy>> s_enemies;
  static std::unordered_map<std::string, std::shared_ptr<Quest>> s_quests;
  static std::unordered_map<std::string, std::shared_ptr<Mod>> s_mods;
  static std::unordered_map<std::string, std::vector<LootEntry>> s_lootTables;

  static void loadDialogues(const std::string& filePath);
  static void loadNPCs(const std::string& filePath);
  static void loadItems(const std::string& filePath);
  static void loadLocations(const std::string& filePath);
  static void loadFactions(const std::string& filePath);
  static void loadEnemies(const std::string& filePath);
  static void loadQuests(const std::string& filePath);
  static void loadMods(const std::string& filePath);
  static void loadLootTable(const std::string& filePath);
};
