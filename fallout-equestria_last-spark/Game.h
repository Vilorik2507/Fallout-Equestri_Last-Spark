#include <memory>
#include <string>

#include "CombatSystem.h"
#include "GameClient.h"
#include "GameState.h"
#include "Location.h"
#include "Player.h"
#include "ResourceManager.h"

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

 private:
  std::unique_ptr<Location> current_location;
  std::unique_ptr<Player> player_object;
  std::unique_ptr<CombatSystem> combat_system;
  std::unique_ptr<GameClient> network_client;
  bool is_multiplayer;
};
