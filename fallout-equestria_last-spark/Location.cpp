#include "Location.h"

#include <iostream>
#include <string>

#include "CombatSystem.h"
#include "Enemy.h"
#include "Game.h"
#include "NPC.h"
#include "Player.h"
#include "Printer.h"

std::shared_ptr<Player> Location::g_player = nullptr;
std::shared_ptr<CombatSystem> Location::g_combatSystem = nullptr;
std::unordered_map<std::string, std::string> Location::tags;
Game* Location::g_game = nullptr;
Location::Location(const std::string& name, const std::string& disc)
    : name_loc(name), description(disc) {};

void Location::addConnection(std::shared_ptr<Location> connection) {
  connections_loc.push_back(std::move(connection));
}

void Location::addNPC(std::shared_ptr<NPC> npc) {
  npc_list.push_back(std::move(npc));
}

void Location::addEnemy(std::shared_ptr<Enemy> enemy) {
  enemies_list.push_back(std::move(enemy));
}

void Location::removeNPC(size_t index) {
  if (index < npc_list.size()) {
    npc_list.erase(npc_list.begin() + index);
  }
}

void Location::removeEnemy(size_t index) {
  if (index < enemies_list.size()) {
    enemies_list.erase(enemies_list.begin() + index);
  }
}
void Location::setGameContext(std::shared_ptr<Player> player,
                              std::shared_ptr<CombatSystem> combatSystem,
                              Game* game) {
  g_player = player;
  g_combatSystem = combatSystem;
  g_game = game;
  tags = {{"{name}", player->getName()}};
}

void Location::onEnter() {
  slow_cout << "\n=== " << name_loc << " ===\n";
  if (description != "") {
    slow_cout.setMode(SlowMode::CharByChar);
    slow_cout.setDelay(25);
    slow_cout << slow_cout.replaceTags(description, tags) << std::endl;
    slow_cout.setDelay(200);
    slow_cout.setMode(SlowMode::LineByLine);
  }
  if (name_loc == "Арена") {
    g_game->startMultiplayerMatch(g_player->getName(), "193.219.117.94", 18080);
    g_game->setMenuShown(true);
    return;
  }
  if (!enemies_list.empty()) {
    if (name_loc == "Груда ржавых машин") {
      g_combatSystem->setWinTargetLocation("highway");
    } else if (name_loc == "Зал Кристального Сердца") {
      g_combatSystem->setWinTargetLocation("final_choice");
    } else if (name_loc == "Лагерь Стальных Жнецов") {
      g_combatSystem->setWinTargetLocation("crystal_empire_gate");
    } else if (name_loc == "Площадь Кристальной Империи") {
      g_combatSystem->setWinTargetLocation("crystal_palace");
    }
    slow_cout << "Вас атакуют враги!\n";
    slow_cout << "Враги:\n";
    for (const auto& enemy : enemies_list) {
      if (enemy) {
        slow_cout << "  - " << enemy->getName() << " (уровень "
                  << enemy->getLevel() << ", HP: " << enemy->getHp() << ")\n";
      }
    }
    if (g_combatSystem && g_player) {
      g_combatSystem->startCombat(g_player, enemies_list);
    }
    return;
  }
  showMenu();
}

void Location::showMenu() {
  while (true) {
    if (g_combatSystem->isCombatActive()) return;
    slow_cout << "\nВы можете:\n";
    int choiceIndex = 1;

    if (!npc_list.empty()) {
      slow_cout << "Поговорить с NPC:\n";
      for (size_t i = 0; i < npc_list.size(); ++i) {
        slow_cout << "  " << choiceIndex++ << ". " << npc_list[i]->getName()
                  << "\n";
      }
    }
    slow_cout << "Уйти:\n";
    std::vector<size_t> connectionIndices;
    for (size_t i = 0; i < connections_loc.size(); ++i) {
      slow_cout << "  " << choiceIndex++ << ". "
                << connections_loc[i]->getName() << "\n";
      connectionIndices.push_back(i);
    }

    int inventoryChoice = choiceIndex;
    slow_cout << "-------------------\n";
    slow_cout << choiceIndex++ << ". Открыть инвентарь\n";
    slow_cout << "Выбор: ";
    if (npc_list.empty() && connections_loc.empty()) {
      slow_cout << "Здесь ничего нет. Возвращайтесь позже.\n";
      break;
    }

    int input;
    std::cin >> input;
    if (input < 1 || input > choiceIndex - 1) {
      slow_cout << "Неверный выбор, попробуйте снова.\n";
      continue;
    }
    if (input == inventoryChoice) {
      if (g_player) {
        showInventory();
      } else {
        slow_cout << "Игрок не найден!\n";
      }
      continue;
    }
    if (!npc_list.empty() && input <= static_cast<int>(npc_list.size())) {
      size_t npcIdx = input - 1;
      npc_list[npcIdx]->talkWithPlayer();
    } else {
      int connIdx = input - 1 - static_cast<int>(npc_list.size());
      if (connIdx >= 0 &&
          static_cast<size_t>(connIdx) < connectionIndices.size()) {
        size_t realIdx = connectionIndices[connIdx];
        std::shared_ptr<Location> nextLoc = connections_loc[realIdx];
        g_game->setLocation(nextLoc);
        break;
      }
    }
  }
}

void Location::showInventory() {
  if (!g_player) return;

  Inventory& inv = g_player->getInventory();
  auto items = inv.getItems();
  if (items.empty()) {
    slow_cout << "Ваш инвентарь пуст.\n";
    return;
  }

  while (true) {
    slow_cout << "\n=== Ваш инвентарь ===\n";
    for (size_t i = 0; i < items.size(); ++i) {
      slow_cout << i + 1 << ". " << items[i]->getName()
                << ", цена: " << items[i]->getValue() << ")\n";
    }
    slow_cout << "0. Назад\n";
    slow_cout << "Выберите предмет для использования (0 для выхода): ";

    int choice;
    std::cin >> choice;
    if (choice == 0) break;

    if (choice < 1 || choice > static_cast<int>(items.size())) {
      slow_cout << "Неверный выбор.\n";
      continue;
    }

    g_player->useItem(choice);
    items = inv.getItems();
    if (items.empty()) {
      slow_cout << "Инвентарь пуст. Возврат в главное меню.\n";
      break;
    }
  }
}
