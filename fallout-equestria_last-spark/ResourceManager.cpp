#include "ResourceManager.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include "Game.h"
#include "Location.h"
#include "Player.h"

std::unordered_map<std::string, std::shared_ptr<Dialogue>>
    ResourceManager::s_dialogues;
std::unordered_map<std::string, std::shared_ptr<NPC>> ResourceManager::s_npcs;
std::unordered_map<std::string, std::shared_ptr<Item>> ResourceManager::s_items;
std::unordered_map<std::string, std::shared_ptr<Location>>
    ResourceManager::s_locations;
std::unordered_map<std::string, std::shared_ptr<Faction>>
    ResourceManager::s_factions;
std::unordered_map<std::string, std::shared_ptr<Enemy>>
    ResourceManager::s_enemies;
std::unordered_map<std::string, std::shared_ptr<Quest>>
    ResourceManager::s_quests;
std::unordered_map<std::string, std::shared_ptr<Mod>> ResourceManager::s_mods;
std::unordered_map<std::string, std::vector<LootEntry>>
    ResourceManager::s_lootTables;

void ResourceManager::init(const std::string& resourcePath) {
  loadFactions(resourcePath + "/factions.json");
  loadItems(resourcePath + "/items.json");
  loadMods(resourcePath + "/mods.json");
  loadQuests(resourcePath + "/quests.json");

  loadDialogues(resourcePath + "/dialogues.json");

  loadLootTable(resourcePath + "/loot_table.json");
  loadEnemies(resourcePath + "/enemies.json");

  loadNPCs(resourcePath + "/npcs.json");

  loadLocations(resourcePath + "/locations.json");
}

std::shared_ptr<Dialogue> ResourceManager::getDialogue(const std::string& id) {
  auto it = s_dialogues.find(id);
  return it != s_dialogues.end() ? it->second : nullptr;
}

std::shared_ptr<NPC> ResourceManager::getNPC(const std::string& id) {
  auto it = s_npcs.find(id);
  return it != s_npcs.end() ? it->second : nullptr;
}

std::shared_ptr<Item> ResourceManager::getItem(const std::string& id) {
  auto it = s_items.find(id);
  return it != s_items.end() ? it->second : nullptr;
}

std::shared_ptr<Location> ResourceManager::getLocation(const std::string& id) {
  auto it = s_locations.find(id);
  return it != s_locations.end() ? it->second : nullptr;
}

std::shared_ptr<Faction> ResourceManager::getFaction(const std::string& id) {
  auto it = s_factions.find(id);
  return it != s_factions.end() ? it->second : nullptr;
}

std::shared_ptr<Enemy> ResourceManager::getEnemy(const std::string& id) {
  auto it = s_enemies.find(id);
  return it != s_enemies.end() ? it->second : nullptr;
}

std::shared_ptr<Quest> ResourceManager::getQuest(const std::string& id) {
  auto it = s_quests.find(id);
  return it != s_quests.end() ? it->second : nullptr;
}

std::shared_ptr<Mod> ResourceManager::getMod(const std::string& id) {
  auto it = s_mods.find(id);
  return it != s_mods.end() ? it->second : nullptr;
}

void ResourceManager::generateLoot(const std::string& lootTableId) {
  auto it = s_lootTables.find(lootTableId);
  if (it == s_lootTables.end()) return;

  auto player = Game::getPlayer();
  if (!player) return;

  for (const auto& entry : it->second) {
    float r = static_cast<float>(rand()) / RAND_MAX;
    if (r <= entry.chance) {
      auto item = getItem(entry.itemId);
      if (item) {
        player->addItem(item);
      }
    }
  }
}

void ResourceManager::cleanup() {
  s_dialogues.clear();
  s_npcs.clear();
  s_items.clear();
  s_locations.clear();
  s_factions.clear();
  s_enemies.clear();
  s_quests.clear();
  s_mods.clear();
  s_lootTables.clear();
}

namespace {
std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(" \t\n\r");
  if (first == std::string::npos) return "";
  size_t last = str.find_last_not_of(" \t\n\r");
  return str.substr(first, last - first + 1);
}

bool parseString(const std::string& json, size_t& pos, std::string& out) {
  if (json[pos] != '"') return false;
  pos++;
  out.clear();
  while (pos < json.size() && json[pos] != '"') {
    if (json[pos] == '\\') {
      pos++;
      if (pos >= json.size()) return false;
      switch (json[pos]) {
        case 'n':
          out += '\n';
          break;
        case 't':
          out += '\t';
          break;
        case '"':
          out += '"';
          break;
        case '\\':
          out += '\\';
          break;
        default:
          out += json[pos];
          break;
      }
    } else {
      out += json[pos];
    }
    pos++;
  }
  if (json[pos] != '"') return false;
  pos++;
  return true;
}

bool parseInt(const std::string& json, size_t& pos, int& out) {
  size_t start = pos;
  if (json[pos] == '-') pos++;
  while (pos < json.size() && std::isdigit(json[pos])) pos++;
  if (start == pos) return false;
  out = std::stoi(json.substr(start, pos - start));
  return true;
}

bool parseFloat(const std::string& json, size_t& pos, float& out) {
  size_t start = pos;
  if (json[pos] == '-') pos++;
  bool hasDot = false;
  while (pos < json.size() && (std::isdigit(json[pos]) || json[pos] == '.')) {
    if (json[pos] == '.') {
      if (hasDot) return false;
      hasDot = true;
    }
    pos++;
  }
  if (start == pos) return false;
  out = std::stof(json.substr(start, pos - start));
  return true;
}

bool parseValue(const std::string& json, size_t& pos, std::string& out) {
  while (pos < json.size() && std::isspace(json[pos])) pos++;
  if (json[pos] == '"') {
    return parseString(json, pos, out);
  } else if (std::isdigit(json[pos]) || json[pos] == '-') {
    size_t start = pos;
    while (pos < json.size() &&
           (std::isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-'))
      pos++;
    out = json.substr(start, pos - start);
    return true;
  } else if (json.substr(pos, 4) == "true") {
    out = "true";
    pos += 4;
    return true;
  } else if (json.substr(pos, 5) == "false") {
    out = "false";
    pos += 5;
    return true;
  } else if (json.substr(pos, 4) == "null") {
    out = "";
    pos += 4;
    return true;
  }
  return false;
}

bool parseObject(const std::string& json, size_t& pos,
                 std::map<std::string, std::string>& out) {
  if (json[pos] != '{') return false;
  pos++;
  out.clear();
  while (true) {
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] == '}') {
      pos++;
      break;
    }
    std::string key;
    if (!parseString(json, pos, key)) return false;
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] != ':') return false;
    pos++;
    std::string value;
    if (!parseValue(json, pos, value)) return false;
    out[key] = value;
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] == ',')
      pos++;
    else if (json[pos] != '}')
      return false;
  }
  return true;
}

bool parseArray(const std::string& json, size_t& pos,
                std::vector<std::map<std::string, std::string>>& out) {
  if (json[pos] != '[') return false;
  pos++;
  out.clear();
  while (true) {
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] == ']') {
      pos++;
      break;
    }
    std::map<std::string, std::string> item;
    if (!parseObject(json, pos, item)) return false;
    out.push_back(item);
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] == ',')
      pos++;
    else if (json[pos] != ']')
      return false;
  }
  return true;
}

bool parseArraySimple(const std::string& json, size_t& pos,
                      std::vector<std::string>& out) {
  if (json[pos] != '[') return false;
  pos++;
  out.clear();
  while (true) {
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] == ']') {
      pos++;
      break;
    }
    std::string value;
    if (!parseValue(json, pos, value)) return false;
    out.push_back(value);
    while (pos < json.size() && std::isspace(json[pos])) pos++;
    if (json[pos] == ',')
      pos++;
    else if (json[pos] != ']')
      return false;
  }
  return true;
}

bool loadFile(const std::string& path, std::string& content) {
  std::ifstream file(path);
  if (!file.is_open()) return false;
  std::stringstream buffer;
  buffer << file.rdbuf();
  content = buffer.str();
  return true;
}
}  // namespace

void ResourceManager::loadFactions(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> factionsList;
  if (!parseArray(content, pos, factionsList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }
  for (auto& data : factionsList) {
    std::string id = data["id"];
    int fid = std::stoi(data["fid"]);
    std::string name = data["name"];
    auto faction = std::make_shared<Faction>(fid, name);
    s_factions[id] = faction;
  }
}

void ResourceManager::loadDialogues(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }

  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> dialoguesList;
  if (!parseArray(content, pos, dialoguesList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }

  for (auto& data : dialoguesList) {
    std::string id = data["id"];
    auto dialogue = std::make_shared<Dialogue>(id);

    if (data.count("startNode")) {
      dialogue->setStartNode(data["startNode"]);
    }

    int nodeIndex = 1;
    while (true) {
      std::string nodeKey = "node" + std::to_string(nodeIndex);
      std::string nodeId = data[nodeKey + "_id"];

      if (nodeId.empty()) break;

      std::string npcLine = data[nodeKey + "_line"];
      std::string choicesStr = data[nodeKey + "_choices"];
      std::vector<Choice> choices;

      if (!choicesStr.empty()) {
        std::stringstream ss(choicesStr);
        std::string choiceItem;

        while (std::getline(ss, choiceItem, ';')) {
          std::stringstream cs(choiceItem);
          std::string text, nextNodeId, questId, repStr;

          std::getline(cs, text, '|');
          std::getline(cs, nextNodeId, '|');
          std::getline(cs, questId, '|');
          std::getline(cs, repStr, '|');

          std::map<Faction*, int> repDelta;

          if (!repStr.empty() && repStr != "-") {
            std::stringstream rs(repStr);
            std::string factionPair;

            while (std::getline(rs, factionPair, ',')) {
              size_t colonPos = factionPair.find(':');
              if (colonPos != std::string::npos) {
                std::string factionId = factionPair.substr(0, colonPos);
                int delta = std::stoi(factionPair.substr(colonPos + 1));
                Faction* faction = getFaction(factionId).get();
                if (faction) {
                  repDelta[faction] = delta;
                }
              }
            }
          }

          Quest* quest = nullptr;
          if (!questId.empty() && questId != "-") {
            quest = getQuest(questId).get();
          }

          choices.push_back(Choice(text, repDelta, nextNodeId, quest));
        }
      }

      DialogueNode* node = new DialogueNode(npcLine, choices);
      dialogue->addNode(nodeId, node);

      nodeIndex++;
    }

    s_dialogues[id] = dialogue;
  }
}

void ResourceManager::loadNPCs(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> npcsList;
  if (!parseArray(content, pos, npcsList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }
  for (auto& data : npcsList) {
    std::string id = data["id"];
    std::string name = data["name"];
    std::string dialogueId = data["dialogue"];
    std::string factionId = data["faction"];
    std::string shopStr = data.count("shop") ? data["shop"] : "";

    Dialogue* dialogue =
        dialogueId.empty() ? nullptr : getDialogue(dialogueId).get();
    Faction* faction =
        factionId.empty() ? nullptr : getFaction(factionId).get();

    auto npc = std::make_shared<NPC>(name, dialogue, faction);

    if (!shopStr.empty()) {
      std::stringstream ss(shopStr);
      std::string itemId;
      while (std::getline(ss, itemId, ',')) {
        itemId = trim(itemId);
        if (itemId.empty()) continue;
        auto item = getItem(itemId);
        if (item) {
          npc->addShopItem(item);

        } else {
          std::cerr << "Shop item not found: " << itemId << " for NPC " << id
                    << std::endl;
        }
      }
    }

    s_npcs[id] = npc;
  }
}

void ResourceManager::loadItems(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> itemsList;
  if (!parseArray(content, pos, itemsList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }
  for (auto& data : itemsList) {
    std::string id = data["id"];
    std::string type = data["type"];
    std::string name = data["name"];
    float weight = data.count("weight") ? std::stof(data["weight"]) : 0.0f;
    int value = data.count("value") ? std::stoi(data["value"]) : 0;
    if (type == "weapon") {
      int damage = data.count("damage") ? std::stoi(data["damage"]) : 0;
      int dist = data.count("dist") ? std::stoi(data["dist"]) : 0;
      auto weapon = std::make_shared<Weapon>(name, weight, value, damage, dist);
      s_items[id] = weapon;
    } else if (type == "armor") {
      int defense = data.count("defense") ? std::stoi(data["defense"]) : 0;
      auto armor = std::make_shared<Armor>(name, weight, value, defense);
      s_items[id] = armor;
    } else if (type == "consumable") {
      int heal = data.count("heal") ? std::stoi(data["heal"]) : 0;
      auto consumable = std::make_shared<Consumable>(name, weight, value, heal);
      s_items[id] = consumable;
    } else {
      auto item = std::make_shared<Item>(name, weight, value);
      s_items[id] = item;
    }
  }
}

void ResourceManager::loadLocations(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> locationsList;
  if (!parseArray(content, pos, locationsList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }

  std::map<std::string, std::vector<std::string>> connectionsMap;
  std::map<std::string, std::vector<std::string>> npcsMap;
  std::map<std::string, std::vector<std::string>> enemiesMap;

  for (auto& data : locationsList) {
    std::string id = data["id"];
    std::string name = data["name"];
    std::string description = data["description"];

    auto location = std::make_shared<Location>(name, description);

    if (data.count("connections")) {
      std::string connStr = data["connections"];
      size_t p = 0;
      if (connStr.size() > 0 && connStr[0] == '[') {
        std::vector<std::string> connIds;
        size_t tempPos = 0;
        parseArraySimple(connStr, tempPos, connIds);
        connectionsMap[id] = connIds;
      } else {
        std::stringstream ss(connStr);
        std::string connId;
        while (std::getline(ss, connId, ',')) {
          connId = trim(connId);
          if (!connId.empty()) connectionsMap[id].push_back(connId);
        }
      }
    }

    if (data.count("npcs")) {
      std::string npcStr = data["npcs"];
      size_t p = 0;
      if (npcStr.size() > 0 && npcStr[0] == '[') {
        std::vector<std::string> npcIds;
        size_t tempPos = 0;
        parseArraySimple(npcStr, tempPos, npcIds);
        npcsMap[id] = npcIds;
      } else {
        std::stringstream ss(npcStr);
        std::string npcId;
        while (std::getline(ss, npcId, ',')) {
          npcId = trim(npcId);
          if (!npcId.empty()) npcsMap[id].push_back(npcId);
        }
      }
    }

    if (data.count("enemies")) {
      std::string enemyStr = data["enemies"];
      size_t p = 0;
      if (enemyStr.size() > 0 && enemyStr[0] == '[') {
        std::vector<std::string> enemyIds;
        size_t tempPos = 0;
        parseArraySimple(enemyStr, tempPos, enemyIds);
        enemiesMap[id] = enemyIds;
      } else {
        std::stringstream ss(enemyStr);
        std::string enemyId;
        while (std::getline(ss, enemyId, ',')) {
          enemyId = trim(enemyId);
          if (!enemyId.empty()) enemiesMap[id].push_back(enemyId);
        }
      }
    }

    s_locations[id] = location;
  }

  for (auto& pair : connectionsMap) {
    auto loc = s_locations[pair.first];
    if (!loc) continue;
    for (const std::string& connId : pair.second) {
      auto conn = s_locations[connId];
      if (conn) loc->addConnection(conn);
    }
  }

  for (auto& pair : npcsMap) {
    auto loc = s_locations[pair.first];
    if (!loc) continue;
    for (const std::string& npcId : pair.second) {
      auto npc = s_npcs[npcId];
      if (npc) {
        loc->addNPC(npc);
      }
    }
  }

  for (auto& pair : enemiesMap) {
    auto loc = s_locations[pair.first];
    if (!loc) continue;
    for (const std::string& enemyId : pair.second) {
      auto enemy = s_enemies[enemyId];
      if (enemy) {
        auto enemyCopy = std::make_shared<Enemy>(
            enemy->getName(), enemy->getLevel(), enemy->getHp(),
            enemy->getDamage(), enemy->getInitiative(),
            enemy->getLootTableId());
        loc->addEnemy(std::move(enemyCopy));
      }
    }
  }
}

void ResourceManager::loadEnemies(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load enemies: " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> enemiesData;
  if (!parseArray(content, pos, enemiesData)) {
    std::cerr << "Error parsing enemies JSON" << std::endl;
    return;
  }
  for (const auto& data : enemiesData) {
    std::string id = data.at("id");
    std::string name = data.at("name");
    int level = std::stoi(data.at("level"));
    int hp = std::stoi(data.at("hp"));
    int damage = std::stoi(data.at("damage"));
    int initiative = std::stoi(data.at("initiative"));
    std::string lootTableId = data.at("loot_table");

    auto enemy = std::make_shared<Enemy>(name, level, hp, damage, initiative,
                                         lootTableId);
    s_enemies[id] = std::move(enemy);
  }
}

void ResourceManager::loadQuests(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> questsList;
  if (!parseArray(content, pos, questsList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }
  for (auto& data : questsList) {
    std::string id = data["id"];
    std::string name = data["name"];
    std::string stagesStr = data["stages"];
    std::vector<QuestStage*> stages;
    if (stagesStr.size() > 0 && stagesStr[0] == '[') {
      size_t p = 0;
      std::vector<std::string> stageList;
      parseArraySimple(stagesStr, p, stageList);
      for (const std::string& desc : stageList) {
        if (!desc.empty()) stages.push_back(new QuestStage(desc));
      }
    } else {
      std::stringstream ss(stagesStr);
      std::string stageDesc;
      while (std::getline(ss, stageDesc, '|')) {
        stageDesc = trim(stageDesc);
        if (!stageDesc.empty()) stages.push_back(new QuestStage(stageDesc));
      }
    }
    auto quest = std::make_shared<Quest>(name, stages);
    s_quests[id] = quest;
  }
}

void ResourceManager::loadMods(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load mods: " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> modsData;
  if (!parseArray(content, pos, modsData)) {
    std::cerr << "Error parsing mods JSON" << std::endl;
    return;
  }
  for (const auto& data : modsData) {
    std::string id = data.at("id");
    std::string bonusesStr = data.at("bonuses");
    std::map<std::string, int> bonuses;
    std::stringstream ss(bonusesStr);
    std::string pair;
    while (std::getline(ss, pair, ',')) {
      size_t colon = pair.find(':');
      if (colon != std::string::npos) {
        std::string key = pair.substr(0, colon);
        int value = std::stoi(pair.substr(colon + 1));
        bonuses[key] = value;
      }
    }
    s_mods[id] = std::make_shared<Mod>(bonuses);
  }
}

void ResourceManager::loadLootTable(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load loot table: " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> lootData;
  if (!parseArray(content, pos, lootData)) {
    std::cerr << "Error parsing loot table JSON" << std::endl;
    return;
  }
  for (const auto& data : lootData) {
    std::string enemyId = data.at("enemy_id");
    std::string lootStr = data.at("loot");
    std::vector<LootEntry> entries;
    std::stringstream ss(lootStr);
    std::string entry;
    while (std::getline(ss, entry, ',')) {
      size_t colon = entry.find(':');
      if (colon != std::string::npos) {
        LootEntry le;
        le.itemId = entry.substr(0, colon);
        le.chance = std::stof(entry.substr(colon + 1));
        entries.push_back(le);
      }
    }
    s_lootTables[enemyId] = std::move(entries);
  }
}
