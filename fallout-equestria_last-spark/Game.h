#pragma once
#include <memory>
#include <string>

#include "ResourceManager.h"
class Location;
class Player;
class CombatSystem;
class GameClient;
class GameState;

class Game {
 public:
  Game();
  ~Game() = default;
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
  Game(Game&&) = default;
  Game& operator=(Game&&) = default;

  void startGame();
  void saveGameState(const GameState& state);
  void loadGame();
  void handleInput();
  void updateDeltaTime();
  void startMultiplayerMatch(const std::string& nickname, const std::string& ip,
                             int port);
  void setLocation(std::shared_ptr<Location> loc);
  static void gameOver();
  static std::shared_ptr<Player> getPlayer() { return player_object; }

 private:
  std::shared_ptr<Location> current_location;
  static std::shared_ptr<Player> player_object;
  std::shared_ptr<CombatSystem> combat_system;
  std::shared_ptr<GameClient> network_client;
  bool is_multiplayer;
  bool menuShown = false;
};
