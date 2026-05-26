#include "Game.h"

#include <iostream>

Game::Game() { current_location = std::make_unique<Location>("loc1"); }

void Game::startGame() {
  ResourceManager::init("resources");
  std::cout << "Добро пожаловать в мир апокалипсиса!";
}
