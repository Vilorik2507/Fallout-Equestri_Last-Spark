#include "CombatSystem.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "CombatAI.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "Printer.h"
#include "ResourceManager.h"

bool CombatSystem::isCombatActive() const { return combat_active; }
bool CombatSystem::isPlayerTurn() const { return player_turn; }
void CombatSystem::setGame(Game* g) { game = g; }

void CombatSystem::setWinTargetLocation(const std::string& locId) {
    winTargetLocation = locId;
}
void CombatSystem::setPlayerTurn(bool turn) {
    if (is_multiplayer) {
        player_turn = turn;
    }
}
void CombatSystem::calculateTurnOrder() {
    std::sort(participants.begin(), participants.end(), [](auto& a, auto& b) {
        return a->getInitiative() > b->getInitiative();
        });
}
void CombatSystem::endPlayerTurn() {
    if (player_turn) {
        player_turn = false;
        nextCombatant();
        processNextTurn();
    }
}

void CombatSystem::startMultiplayerCombat(
    std::shared_ptr<Player> local_player,
    std::shared_ptr<NetworkPlayer> opponent) {
  if (!local_player || !opponent) return;
  player = local_player;
  combat_active = true;
  is_multiplayer = true;
  participants.clear();
  participants.push_back(local_player);
  participants.push_back(opponent);
  calculateTurnOrder();
  current_index = -1;
  processNextTurn();
}
void CombatSystem::startCombat(std::shared_ptr<Player> pl,
                               std::vector<std::shared_ptr<Enemy>> enemies) {
  if (!pl || enemies.empty()) return;
  player = pl;
  combat_active = true;
  player_turn = false;
  participants.clear();
  participants.push_back(player);
  for (auto& e : enemies) {
    if (e) {
      int dist = 3;
      e->setDistance(dist);
      participants.push_back(e);
    }
  }
  calculateTurnOrder();
  current_index = -1;
}
void CombatSystem::nextCombatant() {
  if (participants.empty()) {
    current_index = -1;
    return;
  }
  int start = (current_index + 1) % participants.size();
  for (int i = 0; i < (int)participants.size(); ++i) {
    int idx = (start + i) % participants.size();
    auto c = participants[idx];
    if (c && c->isAlive()) {
      current_index = idx;
      return;
    }
  }
  current_index = -1;
  endCombat();
}
void CombatSystem::processNextTurn() {
  if (is_multiplayer) return;
  if (!combat_active) return;
  if (player_turn) return;
  if (current_index < 0 || !participants[current_index]->isAlive()) {
    nextCombatant();
  }
  if (current_index < 0) {
    endCombat();
    return;
  }

  auto current = participants[current_index];
  if (!current || !current->isAlive()) {
    nextCombatant();
    if (current_index < 0) endCombat();
    return;
  }

  if (current == player) {
    player_turn = true;
    slow_cout << "\n=== Твой ход ===" << std::endl;
  } else {
    auto enemy = std::dynamic_pointer_cast<Enemy>(current);
    if (enemy) {
      slow_cout << "\n=== " << enemy->getName()
                << "'s turn (distance: " << enemy->getDistance()
                << ") ===" << std::endl;
      CombatAI ai;
      std::vector<std::shared_ptr<Combatant>> opponents = {player};
      CombatAction action = ai.chooseAction(enemy.get(), {}, opponents);
      executeAction(action);
    }
    nextCombatant();
  }
}

void CombatSystem::executeAction(const CombatAction& action) {
  if (action.getType() == "move_towards") {
    auto* player = dynamic_cast<Player*>(action.getSource());
    auto* target_enemy = dynamic_cast<Enemy*>(action.getTarget());
    if (player && target_enemy) {
      int old_dist = target_enemy->getDistance();
      int new_dist = old_dist - 1;
      if (new_dist < 1) new_dist = 1;
      target_enemy->setDistance(new_dist);
      slow_cout << player->getName() << " moves closer to "
                << target_enemy->getName() << "! Distance: " << new_dist
                << std::endl;
    }
    return;
  }

  if (action.getType() == "move") {
    auto* enemy = dynamic_cast<Enemy*>(action.getSource());
    if (enemy) {
      int old_dist = enemy->getDistance();
      int new_dist = old_dist - 1;
      if (new_dist < 1) new_dist = 1;
      enemy->setDistance(new_dist);
      slow_cout << enemy->getName() << " moves closer! Distance: " << new_dist
                << std::endl;
    }
    return;
  }

  if (action.getType() == "attack") {
    bool too_far = false;
    short max_dist = 2;
    if (player_turn && player->getActivWeopon()) {
      max_dist = player->getActivWeopon()->getDist();
    }
    if (auto* enemy = dynamic_cast<Enemy*>(action.getSource())) {
      if (enemy->getDistance() > max_dist) {
        slow_cout << enemy->getName()
                  << " is too far to attack! Distance: " << enemy->getDistance()
                  << std::endl;
        too_far = true;
      }
    }
    if (too_far) {
      if (auto* enemy = dynamic_cast<Enemy*>(action.getSource())) {
        int new_dist = enemy->getDistance() - 1;
        if (new_dist < 1) new_dist = 1;
        enemy->setDistance(new_dist);
        slow_cout << enemy->getName()
                  << " moves closer instead! Distance: " << new_dist
                  << std::endl;
      }
      return;
    }

    if (!action.calculateHit()) {
      slow_cout << action.getSource()->getName() << " misses!\n";
    } else {
      action.apply();
    }
    if (!action.getTarget()->isAlive()) {
      slow_cout << action.getTarget()->getName() << " dies!\n";
      auto* enemy = dynamic_cast<Enemy*>(action.getTarget());
      if (enemy) {
        ResourceManager::generateLoot(enemy->getLootTableId());
        int expReward = 10;
        if (enemy->getName() == "Главарь бандитов" ||
            enemy->getName() == "Король Сомбра")
          expReward = 50;
        player->addExp(expReward);
        slow_cout << "Вы получили " << expReward << " опыта.\n";
      }
    }
  }

  bool any_enemy_alive = false;
  for (auto& p : participants) {
    if (auto e = std::dynamic_pointer_cast<Enemy>(p)) {
      if (e->isAlive()) any_enemy_alive = true;
    }
  }
  if (!any_enemy_alive) {
    endCombat(true); 
  } else if (!player->isAlive()) {
    endCombat(false); 
  }
}

void CombatSystem::endCombat(bool victory) {
  combat_active = false;
  player_turn = false;
  participants.clear();
  current_index = -1;
  slow_cout << "Combat ended.\n";

  if (!game) {
    slow_cout << "Error: game pointer is null!\n";
    return;
  }

  if (victory) {
    if (winTargetLocation == "final_choice") {
      game->startFinalChoice();
    } else {
      auto target = ResourceManager::getLocation(winTargetLocation);
      if (target) {
        game->setLocation(target);
        slow_cout << "Переход на локацию: " << winTargetLocation << std::endl;
      } else {
        slow_cout << "Локация не найдена: " << winTargetLocation << std::endl;
      }
    }
  } else {
    slow_cout << "Вы погибли. Игра завершена.\n";
    std::exit(0);
  }
}

std::vector<std::shared_ptr<Enemy>> CombatSystem::getLivingEnemies() const {
  std::vector<std::shared_ptr<Enemy>> result;
  for (auto& p : participants) {
    if (auto e = std::dynamic_pointer_cast<Enemy>(p)) {
      if (e->isAlive()) result.push_back(e);
    }
  }
  return result;
}