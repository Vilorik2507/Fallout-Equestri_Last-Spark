#include "Game.h"

#include <iostream>
#include <limits>

#include "CombatAction.h"
#include "CombatSystem.h"
#include "Enemy.h"
#include "Game.h"
#include "GameClient.h"
#include "GameState.h"
#include "Location.h"
#include "NPC.h"
#include "Player.h"
#include "Printer.h"

std::shared_ptr<Player> Game::player_object;

Game::Game() {
  current_location = std::make_unique<Location>("village_center");
  if (!player_object) {
    player_object = std::make_shared<Player>();
  }
  combat_system = std::make_unique<CombatSystem>();
}

void Game::startGame() {
  ResourceManager::init("resources");
  Location::setGameContext(player_object, combat_system, this);
  current_location = ResourceManager::getLocation("village_center");
  current_location->onEnter();
}

void Game::handleInput() {
  if (combat_system && combat_system->isCombatActive()) {
    if (combat_system->isPlayerTurn()) {
      slow_cout << "\n[БОЙ] Твой ход:\n";
      slow_cout << "1. Атаковать\n2. Открыть инвентарь\n3. Убегать\n4. "
                   "Приближаться\nВыбор: ";
      int choice;
      std::cin >> choice;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
      }

      auto enemies = combat_system->getLivingEnemies();
      if (enemies.empty()) return;

      switch (choice) {
        case 1: {
          slow_cout << "Цель:\n";
          for (size_t i = 0; i < enemies.size(); ++i) {
            slow_cout << i + 1 << ". " << enemies[i]->getName()
                      << " (HP: " << enemies[i]->getHp()
                      << ", distance: " << enemies[i]->getDistance() << ")\n";
          }
          slow_cout << "Выбор: ";
          int targetIdx;
          std::cin >> targetIdx;
          if (targetIdx >= 1 && targetIdx <= (int)enemies.size()) {
            CombatAction action(player_object.get(),
                                enemies[targetIdx - 1].get(), "attack");
            combat_system->executeAction(action);
            combat_system->endPlayerTurn();
          }
          break;
        }
        case 2: {
          Inventory& inv = player_object->getInventory();
          auto items = inv.getItems();
          if (items.empty()) {
            slow_cout << "Инвентарь пуст.\n";
            break;
          }
          slow_cout << "Инвентарь:\n";
          inv.listItems();
          slow_cout << "Выбор (0 для выхода): ";
          int itemIdx;
          std::cin >> itemIdx;
          if (itemIdx >= 1 && itemIdx <= (int)items.size()) {
            player_object->useItem(itemIdx);
            combat_system->endPlayerTurn();
          }
          break;
        }
        case 3: {
          slow_cout << "Вы отдаляетесь!\n";
          bool any_close = false;
          for (auto& enemy : enemies) {
            int new_dist = enemy->getDistance() + 2;
            if (new_dist > 15) new_dist = 15;
            enemy->setDistance(new_dist);
            slow_cout << enemy->getName() << " Расстояние: " << new_dist
                      << std::endl;
            if (new_dist <= 10) any_close = true;
          }
          if (!any_close) {
            slow_cout << "Вы успешно убегаете!\n";
            combat_system->endCombat();
          } else {
            combat_system->endPlayerTurn();
          }
          break;
        }
        case 4: {
          slow_cout << "К кому приближаться?\n";
          for (size_t i = 0; i < enemies.size(); ++i) {
            slow_cout << i + 1 << ". " << enemies[i]->getName()
                      << " (distance: " << enemies[i]->getDistance() << ")\n";
          }
          slow_cout << "Выбор: ";
          int targetIdx;
          std::cin >> targetIdx;
          if (targetIdx >= 1 && targetIdx <= (int)enemies.size()) {
            CombatAction action(player_object.get(),
                                enemies[targetIdx - 1].get(), "move_towards");
            combat_system->executeAction(action);
          }
          combat_system->endPlayerTurn();
          break;
        }
        default:
          slow_cout << "Неверный выбор.\n";
      }
    }
    return;
  }
}

void Game::updateDeltaTime() {
  if (combat_system && combat_system->isCombatActive()) {
    while (!combat_system->isPlayerTurn() && combat_system->isCombatActive()) {
      combat_system->processNextTurn();
    }
    return;
  } else if (current_location && !menuShown) {
    current_location->showMenu();
    menuShown = true;
  }
}

void Game::setLocation(std::shared_ptr<Location> loc) {
  loc->onEnter();
  menuShown = false;
  current_location = loc;
}
