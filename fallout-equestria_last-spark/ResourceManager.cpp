#include "ResourceManager.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

std::unordered_map<std::string, std::unique_ptr<Dialogue>>
    ResourceManager::s_dialogues;
std::unordered_map<std::string, std::unique_ptr<NPC>> ResourceManager::s_npcs;
std::unordered_map<std::string, std::unique_ptr<Item>> ResourceManager::s_items;
std::unordered_map<std::string, std::unique_ptr<Location>>
    ResourceManager::s_locations;
std::unordered_map<std::string, std::unique_ptr<Faction>>
    ResourceManager::s_factions;
std::unordered_map<std::string, std::unique_ptr<Enemy>>
    ResourceManager::s_enemies;
std::unordered_map<std::string, std::unique_ptr<Quest>>
    ResourceManager::s_quests;

void ResourceManager::init(const std::string& resourcePath) {
  loadFactions(resourcePath + "/factions.json");
  loadDialogues(resourcePath + "/dialogues.json");
  loadNPCs(resourcePath + "/npcs.json");
  loadItems(resourcePath + "/items.json");
  loadLocations(resourcePath + "/locations.json");
  loadEnemies(resourcePath + "/enemies.json");
  loadQuests(resourcePath + "/quests.json");
}

Enemy* ResourceManager::getEnemy(const std::string& id) {
  auto it = s_enemies.find(id);
  return it != s_enemies.end() ? it->second.get() : nullptr;
}

Quest* ResourceManager::getQuest(const std::string& id) {
  auto it = s_quests.find(id);
  return it != s_quests.end() ? it->second.get() : nullptr;
}

Dialogue* ResourceManager::getDialogue(const std::string& id) {
  auto it = s_dialogues.find(id);
  return it != s_dialogues.end() ? it->second.get() : nullptr;
}

NPC* ResourceManager::getNPC(const std::string& id) {
  auto it = s_npcs.find(id);
  return it != s_npcs.end() ? it->second.get() : nullptr;
}

Item* ResourceManager::getItem(const std::string& id) {
  auto it = s_items.find(id);
  return it != s_items.end() ? it->second.get() : nullptr;
}

Location* ResourceManager::getLocation(const std::string& id) {
  auto it = s_locations.find(id);
  return it != s_locations.end() ? it->second.get() : nullptr;
}

Faction* ResourceManager::getFaction(const std::string& id) {
  auto it = s_factions.find(id);
  return it != s_factions.end() ? it->second.get() : nullptr;
}

void ResourceManager::cleanup() {
  s_dialogues.clear();
  s_npcs.clear();
  s_items.clear();
  s_locations.clear();
  s_factions.clear();
  s_enemies.clear();
  s_quests.clear();
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

bool loadFile(const std::string& path, std::string& content) {
  std::ifstream file(path);
  if (!file.is_open()) return false;
  std::stringstream buffer;
  buffer << file.rdbuf();
  content = buffer.str();
  if (content.size() >= 3 && (unsigned char)content[0] == 0xEF &&
      (unsigned char)content[1] == 0xBB && (unsigned char)content[2] == 0xBF) {
    content.erase(0, 3);
  }
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
    auto faction = std::make_unique<Faction>(fid, name);
    s_factions[id] = std::move(faction);
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
    auto dialogue = std::make_unique<Dialogue>(id);
    s_dialogues[id] = std::move(dialogue);
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
    Dialogue* dialogue = dialogueId.empty() ? nullptr : getDialogue(dialogueId);
    Faction* faction = factionId.empty() ? nullptr : getFaction(factionId);
    auto npc = std::make_unique<NPC>(name, dialogue, faction);
    s_npcs[id] = std::move(npc);
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
      auto weapon = std::make_unique<Weapon>(name, weight, value, damage);
      s_items[id] = std::move(weapon);
    } else if (type == "armor") {
      int defense = data.count("defense") ? std::stoi(data["defense"]) : 0;
      auto armor = std::make_unique<Armor>(name, weight, value, defense);
      s_items[id] = std::move(armor);
    } else if (type == "consumable") {
      int heal = data.count("heal") ? std::stoi(data["heal"]) : 0;
      auto consumable = std::make_unique<Consumable>(name, weight, value, heal);
      s_items[id] = std::move(consumable);
    } else {
      auto item = std::make_unique<Item>(name, weight, value);
      s_items[id] = std::move(item);
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
  for (auto& data : locationsList) {
    std::string id = data["id"];
    std::string name = data["name"];
    auto location = std::make_unique<Location>(name);
    s_locations[id] = std::move(location);
  }
}

void ResourceManager::loadEnemies(const std::string& filePath) {
  std::string content;
  if (!loadFile(filePath, content)) {
    std::cerr << "Failed to load " << filePath << std::endl;
    return;
  }
  size_t pos = 0;
  std::vector<std::map<std::string, std::string>> enemiesList;
  if (!parseArray(content, pos, enemiesList)) {
    std::cerr << "Error parsing " << filePath << std::endl;
    return;
  }
  for (auto& data : enemiesList) {
    std::string id = data["id"];
    std::string name = data["name"];
    int level = std::stoi(data["level"]);
    int hp = std::stoi(data["hp"]);
    int damage = std::stoi(data["damage"]);
    int initiative = std::stoi(data["initiative"]);
    auto enemy = std::make_unique<Enemy>(name, level, hp, damage, initiative);
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
    std::string stagesStr = data["stages"];  // например, "stage1,stage2,stage3"
    // Разбираем этапы (простейший вариант, можно расширить)
    std::vector<QuestStage*> stages;
    std::stringstream ss(stagesStr);
    std::string stageDesc;
    while (std::getline(ss, stageDesc, '|')) {  // разделитель | для описаний
      if (!stageDesc.empty()) {
        stages.push_back(new QuestStage(stageDesc));
      }
    }
    auto quest = std::make_unique<Quest>(name, stages);
    s_quests[id] = std::move(quest);
  }
}
