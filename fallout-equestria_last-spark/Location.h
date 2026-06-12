#ifndef LOCATION_H
#define LOCATION_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Player;
class CombatSystem;
class Game;
class NPC;
class Enemy;

class Location {
 public:
  Location(const std::string& name, const std::string& disc = "",
           const std::string& id = "");
  ~Location() = default;

  static void setGameContext(std::shared_ptr<Player> player,
                             std::shared_ptr<CombatSystem> combatSystem,
                             Game* game);

  void onEnter();
  std::string getName() const { return name_loc; }
  const std::vector<std::shared_ptr<Location>>& getConnections() const {
    return connections_loc;
  }
  const std::vector<std::shared_ptr<NPC>>& getNPCList() const {
    return npc_list;
  }
  const std::vector<std::shared_ptr<Enemy>>& getEnemiesList() const {
    return enemies_list;
  }

  void addConnection(std::shared_ptr<Location> connection);
  void addNPC(std::shared_ptr<NPC> npc);
  void addEnemy(std::shared_ptr<Enemy> enemy);
  void removeNPC(size_t index);
  void removeEnemy(size_t index);
  void showMenu();
  void showInventory();

 private:
  static std::shared_ptr<Player> g_player;
  static std::shared_ptr<CombatSystem> g_combatSystem;
  static Game* g_game;
  static std::unordered_map<std::string, std::string> tags;

  std::string loc_id;
  std::string name_loc;
  std::string description;
  std::vector<std::shared_ptr<Location>> connections_loc;
  std::vector<std::shared_ptr<NPC>> npc_list;
  std::vector<std::shared_ptr<Enemy>> enemies_list;
};

#endif  // !LOCATION_H
