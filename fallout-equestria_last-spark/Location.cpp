#include "Location.h"

#include <iostream>
#include <string>

#include "CombatSystem.h"
#include "Enemy.h"
#include "Game.h"
#include "NPC.h"
#include "Player.h"

std::shared_ptr<Player> Location::g_player = nullptr;
std::shared_ptr<CombatSystem> Location::g_combatSystem = nullptr;
Game* Location::g_game = nullptr;

void Location::setGameContext(std::shared_ptr<Player> player,
                              std::shared_ptr<CombatSystem> combatSystem,
                              Game* game) {
  g_player = player;
  g_combatSystem = combatSystem;
  g_game = game;
}

Location::Location(const std::string& name) : name_loc(name) {};

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

void Location::onEnter() {
  std::cout << "\n=== " << name_loc << " ===\n";
  if (!enemies_list.empty()) {
    std::cout << "Вас атакуют враги!\n";
    if (g_combatSystem && g_player) {
      g_combatSystem->startCombat(g_player, enemies_list);
    }
    return;  // не показываем меню, пока идёт бой
  }
  showMenu();  // если врагов нет – сразу показываем меню
}

void Location::showMenu() {
  while (true) {
    std::cout << "\nЗдесь вы можете:\n";
    int choiceIndex = 1;

    if (!npc_list.empty()) {
      std::cout << "Поговорить с NPC:\n";
      for (size_t i = 0; i < npc_list.size(); ++i) {
        std::cout << "  " << choiceIndex++ << ". " << npc_list[i]->getName()
                  << "\n";
      }
    }
    std::cout << "Уйти:\n";
    std::vector<size_t> connectionIndices;
    for (size_t i = 0; i < connections_loc.size(); ++i) {
      std::cout << "  " << choiceIndex++ << ". "
                << connections_loc[i]->getName() << "\n";
      connectionIndices.push_back(i);
    }

    if (npc_list.empty() && connections_loc.empty()) {
      std::cout << "Здесь ничего нет. Возвращайтесь позже.\n";
      break;
    }

    int input;
    std::cin >> input;
    if (input < 1 || input > choiceIndex - 1) {
      std::cout << "Неверный выбор, попробуйте снова.\n";
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
