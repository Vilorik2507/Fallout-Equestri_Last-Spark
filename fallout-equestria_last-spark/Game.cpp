#include "Game.h"

#include <iostream>

#include "CombatSystem.h"
#include "GameClient.h"
#include "GameState.h"
#include "Location.h"
#include "Player.h"

std::shared_ptr<Player> Game::player_object;

Game::Game() {
  current_location = std::make_unique<Location>("village_center");
  if (!player_object) {
    player_object = std::make_shared<Player>();
  }
}

void Game::startGame() {
  ResourceManager::init("resources");
  current_location = ResourceManager::getLocation("village_center");
  current_location->onEnter();
  std::cout << "Добро пожаловать в мир апокалипсиса!";
}

void Game::setLocation(std::shared_ptr<Location> loc) {
  loc->onEnter();
  current_location = loc;
}
