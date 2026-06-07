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
  std::cout << "Добро пожаловать в мир апокалипсиса!";
}

void Game::handleInput() {
  if (combat_system && combat_system->isCombatActive()) {
    if (combat_system->isPlayerTurn()) {
      std::cout << "\n[Combat] Your turn. Choose action:\n";
      std::cout
          << "1. Attack\n2. Use item\n3. Run away\n4. Approach enemy\nChoice: ";
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
          // Выбор цели с отображением расстояния
          std::cout << "Targets:\n";
          for (size_t i = 0; i < enemies.size(); ++i) {
            std::cout << i + 1 << ". " << enemies[i]->getName()
                      << " (HP: " << enemies[i]->getHp()
                      << ", distance: " << enemies[i]->getDistance() << ")\n";
          }
          std::cout << "Choice: ";
          int targetIdx;
          std::cin >> targetIdx;
          if (targetIdx >= 1 && targetIdx <= (int)enemies.size()) {
            CombatAction action(player_object.get(),
                                enemies[targetIdx - 1].get(), "attack");
            combat_system->executeAction(action);
            // executeAction уже вызовет processNextTurn после хода игрока
            combat_system->endPlayerTurn();
          }
          break;
        }
        case 2: {
          // Использовать предмет – показать инвентарь
          Inventory& inv = player_object->getInventory();
          auto items = inv.getItems();
          if (items.empty()) {
            std::cout << "No items to use.\n";
            break;
          }
          std::cout << "Choose item to use:\n";
          inv.listItems();
          std::cout << "Choice (0 to cancel): ";
          int itemIdx;
          std::cin >> itemIdx;
          if (itemIdx >= 1 && itemIdx <= (int)items.size()) {
            player_object->useItem(itemIdx);
            // после использования предмета ход не заканчивается?
            // Обычно ход переходит, поэтому вызываем завершение хода
            combat_system->endPlayerTurn();
          }
          break;
        }
        case 3:  // Run away – увеличиваем дистанцию
        {
          std::cout << "You run away!\n";
          bool any_close = false;
          for (auto& enemy : enemies) {
            int new_dist = enemy->getDistance() + 2;
            if (new_dist > 15) new_dist = 15;
            enemy->setDistance(new_dist);
            std::cout << enemy->getName() << " distance: " << new_dist
                      << std::endl;
            if (new_dist <= 10) any_close = true;
          }
          if (!any_close) {
            std::cout << "You successfully escaped!\n";
            combat_system->endCombat();
          } else {
            // Заканчиваем ход игрока, бой продолжается
            combat_system->endPlayerTurn();
          }
          break;
        }
        case 4:  // Approach enemy
        {
          std::cout << "Approach which enemy?\n";
          for (size_t i = 0; i < enemies.size(); ++i) {
            std::cout << i + 1 << ". " << enemies[i]->getName()
                      << " (distance: " << enemies[i]->getDistance() << ")\n";
          }
          std::cout << "Choice: ";
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
          std::cout << "Invalid choice.\n";
      }
    }
    return;
  }
}

void Game::updateDeltaTime() {
  if (combat_system && combat_system->isCombatActive()) {
    // Пока ход не игрока – продолжаем обрабатывать ходы ИИ
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
