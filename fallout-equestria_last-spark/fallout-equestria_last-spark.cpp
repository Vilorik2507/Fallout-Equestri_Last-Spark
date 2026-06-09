#include <conio.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Game.h"
#include "NetworkManager.h"

int main() {
#ifdef _WIN32
  SetConsoleOutputCP(1251);
  SetConsoleCP(1251);
  system("color a");
#endif
  Game game = Game();
  game.startGame();
  while (true) {
    game.handleInput();
    game.updateDeltaTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}
