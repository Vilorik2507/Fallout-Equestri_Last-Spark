#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Enemy.h"
#include "NPC.h"

class Location {
 public:
  Location() = default;
  Location(const std::string& name);
  ~Location() = default;

  Location(const Location&) = delete;
  Location& operator=(const Location&) = delete;

  Location(Location&&) = default;
  Location& operator=(Location&&) = default;

  void onEnter();
  std::string getName() const { return m_name_loc; }
  const std::vector<std::unique_ptr<Location>>& getConnections() const {
    return connections_loc;
  }
  const std::vector<std::unique_ptr<NPC>>& getNPCList() const {
    return npc_list;
  }
  const std::vector<std::unique_ptr<Enemy>>& getEnemiesList() const {
    return enemies_list;
  }

  void setName(const std::string& name) { m_name_loc = name; }
  void addConnection(std::unique_ptr<Location> connection);
  void addNPC(std::unique_ptr<NPC> npc);
  void addEnemy(std::unique_ptr<Enemy> enemy);
  void removeNPC(size_t index);
  void removeEnemy(size_t index);

 private:
  std::string m_name_loc;
  std::vector<std::unique_ptr<Location>> connections_loc;
  std::vector<std::unique_ptr<NPC>> npc_list;
  std::vector<std::unique_ptr<Enemy>> enemies_list;
};
