#pragma once
#include <memory>
#include <string>

#include "NetworkManager.h"
#include "NetworkPlayer.h"
#include "ResourceManager.h"
class Location;
class Player;
class CombatSystem;
class GameClient;
class GameState;

class Game {
 public:
  void processNetworkMessage(const std::string& msg);
  void sendCombatAction(const CombatAction& action);
  void startMultiplayerMatch(const std::string& nickname, const std::string& ip,
                             int port);
  void sendMove(bool closer);

  void sendRoll();
  void determineFirstTurn();

  Game();
  ~Game() = default;
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
  Game(Game&&) = default;
  Game& operator=(Game&&) = default;

  void startGame();
  void saveGameState(const GameState& state);
  void setMenuShown(bool shown) { menuShown = shown; }
  void loadGame();
  void handleInput();
  void handleMultiplayerTurn();
  void updateDeltaTime();
  void setLocation(std::shared_ptr<Location> loc);
  static void gameOver();
  static std::shared_ptr<Player> getPlayer() { return player_object; }

 private:
  std::chrono::steady_clock::time_point m_match_start_time;
  std::chrono::milliseconds m_my_wait_time{0};

  std::shared_ptr<Location> current_location;
  static std::shared_ptr<Player> player_object;
  std::shared_ptr<CombatSystem> combat_system;
  std::shared_ptr<GameClient> network_client;
  bool is_multiplayer;
  bool menuShown = false;
  bool searching = false;
  std::string m_opponent_name;
  std::shared_ptr<NetworkPlayer> m_network_opponent;
  NetworkManager* m_network_manager = nullptr;
  bool m_waiting_for_roll = false;
  int m_my_roll = -1;
  int m_opponent_roll = -1;
  bool m_roll_processed = false;
};
