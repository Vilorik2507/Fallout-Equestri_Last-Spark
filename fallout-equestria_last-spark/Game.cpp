#include "Game.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <random>

#include "CombatAction.h"
#include "CombatSystem.h"
#include "Enemy.h"
#include "GameClient.h"
#include "GameState.h"
#include "Location.h"
#include "NPC.h"
#include "Player.h"
#include "Printer.h"

std::shared_ptr<Player> Game::player_object;

Game::Game() {
  current_location = ResourceManager::getLocation("vault_room");
  if (!player_object) {
    player_object = std::make_shared<Player>();
  }
  combat_system = std::make_unique<CombatSystem>();
  combat_system->setGame(this);
}

int generateRandomNumber() {
  static std::mt19937 rng(static_cast<unsigned>(
      std::chrono::steady_clock::now().time_since_epoch().count()));
  static std::uniform_int_distribution<int> dist(1, 100);
  return dist(rng);
}

void Game::sendRoll() {
  m_my_roll = generateRandomNumber();
  m_network_manager->Send("ROLL|" + std::to_string(m_my_roll));
  slow_cout << "[MP] Sent roll: " << m_my_roll << "\n";
}
void Game::setLocation(std::shared_ptr<Location> loc) {
  menuShown = false;
  if (loc) {
    loc->onEnter();
  }
  current_location = loc;
}
void Game::startGame() {
  std::string player_name;
  slow_cout << "Enter your name: ";
  std::getline(std::cin, player_name);

  if (player_name.empty()) {
    player_name = "Понька" + std::to_string(generateRandomNumber());
  }
  player_object->setName(player_name);
  ResourceManager::init("resources");
  Location::setGameContext(player_object, combat_system, this);

  current_location = ResourceManager::getLocation("vault_room");
  current_location->onEnter();
}
void Game::determineFirstTurn() {
  if (m_my_roll == -1 || m_opponent_roll == -1) return;
  if (m_roll_processed) return;

  if (m_my_roll > m_opponent_roll) {
    slow_cout << "[MP] You go first! (" << m_my_roll << " vs "
              << m_opponent_roll << ")\n";
    combat_system->startMultiplayerCombat(player_object, m_network_opponent);
    combat_system->setPlayerTurn(true);
  } else if (m_opponent_roll > m_my_roll) {
    slow_cout << "[MP] Opponent goes first! (" << m_opponent_roll << " vs "
              << m_my_roll << ")\n";
    combat_system->startMultiplayerCombat(player_object, m_network_opponent);
    combat_system->setPlayerTurn(false);
  } else {
    slow_cout << "[MP] Tie! (" << m_my_roll << "). Rolling again...\n";
    m_my_roll = -1;
    m_opponent_roll = -1;
    sendRoll();
    return;
  }
  m_roll_processed = true;
  m_waiting_for_roll = false;
}

void Game::startMultiplayerMatch(const std::string& nickname,
                                 const std::string& ip, int port) {
  if (is_multiplayer) return;
  m_network_manager = &NetworkManager::GetInstance();
  m_network_manager->SetMessageCallback(
      [this](const std::string& msg) { processNetworkMessage(msg); });
  m_network_manager->SetConnectCallback(
      []() { slow_cout << "[MP] Connected to match server.\n"; });
  m_network_manager->SetDisconnectCallback(
      []() { slow_cout << "[MP] Disconnected from server.\n"; });

  if (!m_network_manager->Initialize()) {
    slow_cout << "[MP] Failed to init network.\n";
    return;
  }
  if (!m_network_manager->Connect(ip, port)) {
    slow_cout << "[MP] Connection failed.\n";
    return;
  }
  m_network_manager->Send("FIND");
  searching = true;
  slow_cout << "[MP] Searching for opponent...\n";
}

void Game::processNetworkMessage(const std::string& msg) {
  if (msg == "WAITING") {
    slow_cout << "[MP] Waiting for opponent...\n";
    searching = true;
    return;
  }

  if (msg == "OPPONENT_DISCONNECTED") {
    slow_cout << "[MP] Opponent disconnected.\n";
    combat_system->endCombat(false);
    is_multiplayer = false;
    setMenuShown(false);
    return;
  }
  std::stringstream ss(msg);
  std::string cmd;
  std::getline(ss, cmd, '|');
  if (cmd == "START") {
    searching = false;
    is_multiplayer = true;
    m_network_opponent = std::make_shared<NetworkPlayer>("Opponent", 100, 10);
    slow_cout << "[MP] Match found! Determining who goes first...\n";

    m_waiting_for_roll = true;
    m_my_roll = -1;
    m_opponent_roll = -1;
    m_roll_processed = false;
    sendRoll();
    return;
  }

  else if (cmd == "ROLL") {
    int roll;
    ss >> roll;
    if (m_waiting_for_roll) {
      m_opponent_roll = roll;
      slow_cout << "[MP] Opponent roll: " << m_opponent_roll << "\n";
      if (m_my_roll != -1) {
        determineFirstTurn();
      }
    }
    return;
  } else if (cmd == "GET_NAME") {
    m_network_manager->Send("NAME|" + player_object->getName());
  } else if (cmd == "NAME") {
    std::string name;
    std::getline(ss, name);
    m_network_opponent->setName(name);
    slow_cout << "Opponent: " << name << "\n";
  } else if (cmd == "ACTION") {
    std::string type, target;
    int damage;
    std::getline(ss, type, '|');
    if (type == "attack") {
      std::getline(ss, target, '|');
      ss >> damage;
      slow_cout << "[MP] Opponent attacks you for " << damage << " damage!\n";
      player_object->applyDamage(damage);
      std::string response = "STATE|" + std::to_string(player_object->getHp()) +
                             "|" + std::to_string(m_network_opponent->getHp());
      m_network_manager->Send(response);
      if (!player_object->isAlive()) {
        slow_cout << "You died! Opponent wins.\n";
        combat_system->endCombat(false);
        is_multiplayer = false;
        m_network_manager->Disconnect();
      } else {
        combat_system->setPlayerTurn(true);
      }
    }
  } else if (cmd == "STATE") {
    int local_hp, opp_hp;
    ss >> local_hp;
    std::string token;
    std::getline(ss, token, '|');
    ss >> opp_hp;
    m_network_opponent->setHp(opp_hp);
  } else if (cmd == "GAME_OVER") {
    std::string winner;
    std::getline(ss, winner, '|');
    slow_cout << "[MP] Game over! Winner: " << winner << "\n";
    bool victory = (winner == player_object->getName());
    combat_system->endCombat(victory);
    setMenuShown(false);
    is_multiplayer = false;
    m_network_manager->Disconnect();
  } else if (cmd == "MOVE") {
    std::string direction;
    int new_distance;
    std::getline(ss, direction, '|');
    ss >> new_distance;
    if (direction == "closer") {
      m_network_opponent->setDistance(new_distance);
      slow_cout << "Opponent moved closer. Distance: " << new_distance << "\n";
    } else if (direction == "away") {
      m_network_opponent->setDistance(new_distance);
      slow_cout << "Opponent moved away. Distance: " << new_distance << "\n";
    }
    combat_system->setPlayerTurn(true);
  }
}

void Game::sendMove(bool closer) {
  if (!is_multiplayer) return;
  int new_dist = closer ? m_network_opponent->getDistance() - 1
                        : m_network_opponent->getDistance() + 1;
  if (new_dist < 1) new_dist = 1;
  if (new_dist > 15) new_dist = 15;
  m_network_opponent->setDistance(new_dist);
  std::string msg = "MOVE|" + std::string(closer ? "closer" : "away") + "|" +
                    std::to_string(new_dist);
  m_network_manager->Send(msg);
  combat_system->setPlayerTurn(false);
}

void Game::sendCombatAction(const CombatAction& action) {
  if (!is_multiplayer) return;
  std::string msg = "ACTION|";
  msg += action.getType() + "|" + action.getTarget()->getName() + "|";
  if (action.getType() == "attack") {
    int damage = 10;
    msg += std::to_string(damage);
  }
  m_network_manager->Send(msg);
  combat_system->setPlayerTurn(false);
}

void Game::handleInput() {
  if (combat_system && combat_system->isCombatActive()) {
    if (combat_system->isPlayerTurn()) {
      if (combat_system->isMultiplayer()) {
        handleMultiplayerTurn();
        return;
      }
      slow_cout << "\n[БОЙ] Твой ход:\n";
      slow_cout << "1. Атаковать\n2. Открыть инвентарь\n3. Убегать\n4. "
                   "Приближаться\nВыбор: ";
      int choice;
      std::cin >> choice;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
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

void Game::handleMultiplayerTurn() {
  if (!combat_system->isPlayerTurn()) {
    slow_cout << "Waiting for opponent's move...\n";
    return;
  }
  std::cout << "\n[Multiplayer Combat] Твой ход:\n";
  std::cout << "1. Атаковать\n2. использовать предмет\n3. Приблизиться\n4. "
               "Отдалиться\n5. Ливнут\nChoice: ";

  int choice;
  std::cin >> choice;
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    return;
  }

  auto opponent = m_network_opponent;
  if (!opponent) return;

  switch (choice) {
    case 1: {
      if (opponent->getDistance() > 2) {
        slow_cout << "Слишком далеко, расстояние: " << opponent->getDistance()
                  << ". Подойдите ближе.\n";
        break;
      }
      int damage = player_object->getDamage();
      opponent->applyDamage(damage);
      slow_cout << "Вы атакуете " << opponent->getName() << " и наносите "
                << damage << " урона!\n";
      slow_cout << opponent->getName() << " HP: " << opponent->getHp() << "\n";

      std::string msg =
          "ACTION|attack|" + opponent->getName() + "|" + std::to_string(damage);
      m_network_manager->Send(msg);

      if (!opponent->isAlive()) {
        slow_cout << "Вы выиграли!\n";
        m_network_manager->Send("GAME_OVER|" + player_object->getName());
        combat_system->endCombat(true);
        setMenuShown(false);
        is_multiplayer = false;
        break;
      }

      combat_system->setPlayerTurn(false);
      break;
    }
    case 3: {
      int new_dist = opponent->getDistance() - 1;
      if (new_dist < 1) new_dist = 1;
      opponent->setDistance(new_dist);
      slow_cout << "Вы смело преближаетесь. Расстояние: " << new_dist << "\n";

      std::string msg = "MOVE|closer|" + std::to_string(new_dist);
      m_network_manager->Send(msg);
      combat_system->setPlayerTurn(false);
      break;
    }
    case 4: {
      int new_dist = opponent->getDistance() + 1;
      if (new_dist > 15) new_dist = 15;
      opponent->setDistance(new_dist);
      slow_cout << "Вы отдаляетесь. Distance: " << new_dist << "\n";

      std::string msg = "MOVE|away|" + std::to_string(new_dist);
      m_network_manager->Send(msg);
      combat_system->setPlayerTurn(false);
      break;
    }
    case 5: {
      slow_cout << "Вы сбегаете, как трус!\n";
      m_network_manager->Send("GAME_OVER|" + opponent->getName());
      combat_system->endCombat(false);
      setMenuShown(false);
      is_multiplayer = false;
      break;
    }
    default:
      slow_cout << "Неправильный выбор.\n";
  }
}

void Game::updateDeltaTime() {
  if (m_network_manager) {
    m_network_manager->Update();
  }
  if (combat_system && combat_system->isCombatActive()) {
    while (!combat_system->isPlayerTurn() && combat_system->isCombatActive()) {
      combat_system->processNextTurn();
      if (m_network_manager) {
        m_network_manager->Update();
      }
    }
    return;
  } else if (current_location && !menuShown && !is_multiplayer) {
    current_location->showMenu();
    menuShown = true;
  }
}

void Game::startFinalChoice() {
  auto d = ResourceManager::getDialogue("final_choice_dialogue");
  slow_cout << "\n" << d->getNode("node1")->getNpcLine() << "\n\n";
  auto choices = d->getNode("node2")->getChoices();
  for (size_t i = 0; i < choices.size(); ++i)
    slow_cout << i + 1 << ". " << choices[i].getText() << "\n";
  int opt;
  std::cin >> opt;
  slow_cout << "\n"
            << d->getNode("node" + std::to_string(opt + 2))->getNpcLine()
            << "\n";
  slow_cout << "\nИгра завершена. Спасибо за прохождение!\n";
  std::exit(0);
}
