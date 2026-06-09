#pragma once

#include <memory>
#include <vector>

#include "CombatAction.h"
#include "NetworkPlayer.h"

class Player;
class Enemy;

class CombatSystem {
 public:
  void startMultiplayerCombat(std::shared_ptr<Player> local_player,
                              std::shared_ptr<NetworkPlayer> opponent);
  void setPlayerTurn(bool turn);
  bool isMultiplayer() const { return is_multiplayer; }

  void startCombat(std::shared_ptr<Player> pl,
                   std::vector<std::shared_ptr<Enemy>> enemies);
  void executeAction(const CombatAction& action);
  void endCombat();
  bool isCombatActive() const;
  bool isPlayerTurn() const;
  void processNextTurn();
  void endPlayerTurn();
  std::vector<std::shared_ptr<Enemy>> getLivingEnemies() const;

 private:
  void calculateTurnOrder();
  void nextCombatant();

  bool is_multiplayer;
  bool combat_active = false;
  bool player_turn = false;
  int current_index = -1;
  std::vector<std::shared_ptr<Combatant>> participants;
  std::shared_ptr<Player> player;
};
