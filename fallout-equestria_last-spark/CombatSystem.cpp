#include "CombatSystem.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "CombatAI.h"
#include "Enemy.h"
#include "Player.h"

void CombatSystem::startCombat(std::shared_ptr<Player> pl,
                               std::vector<std::shared_ptr<Enemy>> enemies) {
  if (!pl || enemies.empty()) return;
  player = pl;
  combat_active = true;
  player_turn = false;
  participants.clear();
  participants.push_back(player);
  // Устанавливаем случайное расстояние для каждого врага и добавляем в
  // участники
  for (auto& e : enemies) {
    if (e) {
      int dist = 1;  // 1-15
      e->setDistance(dist);
      participants.push_back(e);
    }
  }
  calculateTurnOrder();
  current_index = -1;
  // Не вызываем processNextTurn здесь – первый ход начнётся в
  // Game::updateDeltaTime
}

void CombatSystem::calculateTurnOrder() {
  std::sort(participants.begin(), participants.end(), [](auto& a, auto& b) {
    return a->getInitiative() > b->getInitiative();
  });
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
  if (!combat_active) return;
  if (player_turn) return;  // ждём ввод игрока

  // Если текущий индекс невалиден или указывает на мёртвого – переключаем на
  // следующего
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
    std::cout << "\n=== Your turn ===" << std::endl;
  } else {
    auto enemy = std::dynamic_pointer_cast<Enemy>(current);
    if (enemy) {
      std::cout << "\n=== " << enemy->getName()
                << "'s turn (distance: " << enemy->getDistance()
                << ") ===" << std::endl;
      CombatAI ai;
      // Передаём только игрока как цель
      std::vector<std::shared_ptr<Combatant>> opponents = {player};
      CombatAction action = ai.chooseAction(enemy.get(), {}, opponents);
      executeAction(action);
    }
    // Переход к следующему участнику (после хода врага)
    nextCombatant();  // <- ключевая строка!
  }
}

void CombatSystem::endPlayerTurn() {
  if (player_turn) {
    player_turn = false;
    nextCombatant();
    processNextTurn();
  }
}

void CombatSystem::executeAction(const CombatAction& action) {
  // Обработка движения
  if (action.getType() == "move_towards") {
    auto* player = dynamic_cast<Player*>(action.getSource());
    auto* target_enemy = dynamic_cast<Enemy*>(action.getTarget());
    if (player && target_enemy) {
      int old_dist = target_enemy->getDistance();
      int new_dist = old_dist - 1;
      if (new_dist < 1) new_dist = 1;
      target_enemy->setDistance(new_dist);
      std::cout << player->getName() << " moves closer to "
                << target_enemy->getName() << "! Distance: " << new_dist
                << std::endl;
    }
    return;  // ход закончен
  }

  if (action.getType() == "move") {
    auto* enemy = dynamic_cast<Enemy*>(action.getSource());
    if (enemy) {
      int old_dist = enemy->getDistance();
      int new_dist = old_dist - 1;
      if (new_dist < 1) new_dist = 1;
      enemy->setDistance(new_dist);
      std::cout << enemy->getName() << " moves closer! Distance: " << new_dist
                << std::endl;
    }
    return;  // ход закончен, processNextTurn будет вызван снова из
             // updateDeltaTime
  }

  // Атака
  if (action.getType() == "attack") {
    // Проверка дистанции (атака возможна только при distance <= 2)
    bool too_far = false;
    if (auto* enemy = dynamic_cast<Enemy*>(action.getSource())) {
      if (enemy->getDistance() > 2) {
        std::cout << enemy->getName()
                  << " is too far to attack! Distance: " << enemy->getDistance()
                  << std::endl;
        too_far = true;
      }
    }
    if (too_far) {
      // Автоматическое приближение (можно убрать, если ИИ сам выберет move)
      if (auto* enemy = dynamic_cast<Enemy*>(action.getSource())) {
        int new_dist = enemy->getDistance() - 1;
        if (new_dist < 1) new_dist = 1;
        enemy->setDistance(new_dist);
        std::cout << enemy->getName()
                  << " moves closer instead! Distance: " << new_dist
                  << std::endl;
      }
      return;
    }

    // Нормальное попадание
    if (!action.calculateHit()) {
      std::cout << action.getSource()->getName() << " misses!\n";
    } else {
      action.apply();
    }
    if (!action.getTarget()->isAlive()) {
      std::cout << action.getTarget()->getName() << " dies!\n";
    }
  }

  // Проверка окончания боя
  bool any_enemy_alive = false;
  for (auto& p : participants) {
    if (auto e = std::dynamic_pointer_cast<Enemy>(p)) {
      if (e->isAlive()) any_enemy_alive = true;
    }
  }
  if (!any_enemy_alive || !player->isAlive()) {
    endCombat();
  }
}

void CombatSystem::endCombat() {
  combat_active = false;
  player_turn = false;
  participants.clear();
  current_index = -1;
  std::cout << "Combat ended.\n";
}

bool CombatSystem::isCombatActive() const { return combat_active; }
bool CombatSystem::isPlayerTurn() const { return player_turn; }

std::vector<std::shared_ptr<Enemy>> CombatSystem::getLivingEnemies() const {
  std::vector<std::shared_ptr<Enemy>> result;
  for (auto& p : participants) {
    if (auto e = std::dynamic_pointer_cast<Enemy>(p)) {
      if (e->isAlive()) result.push_back(e);
    }
  }
  return result;
}
