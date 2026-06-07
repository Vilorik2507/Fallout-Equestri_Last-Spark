#include <conio.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Game.h"
#include "NetworkManager.h"

/*void OnMessageReceived(const std::string& message) {
  slow_cout << "\r[<-] " << message << std::endl;
  slow_cout << "> " << std::flush;
}

void OnConnected() {
  slow_cout << "\r[+] Connected to server!" << std::endl;
  slow_cout << "Commands: /find, /quit, /disconnect" << std::endl;
  slow_cout << "> " << std::flush;
}

void OnDisconnected() {
  slow_cout << "\r[-] Disconnected from server" << std::endl;
}
*/

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

/*network.SetMessageCallback(OnMessageReceived);
network.SetConnectCallback(OnConnected);
network.SetDisconnectCallback(OnDisconnected);

std::string input;
bool running = true;

while (running) {
    if (!network.IsConnected()) {
        slow_cout << "Enter server address (e.g. 127.0.0.1:8080 or /quit): ";
        std::getline(std::cin, input);

        if (input == "/quit") {
            running = false;
            break;
        }

        size_t colon_pos = input.find(':');
        std::string host = "127.0.0.1";
        int port = 8080;

        if (colon_pos != std::string::npos) {
            host = input.substr(0, colon_pos);
            port = std::stoi(input.substr(colon_pos + 1));
        }
        else if (!input.empty()) {
            host = input;
        }

        if (!network.Connect(host, port)) {
            slow_cout << "Connection failed. Try again." << std::endl;
            continue;
        }
    }

    // Неблокирующая проверка ввода с консоли
    if (_kbhit()) {
        slow_cout << "> " << std::flush;
        std::getline(std::cin, input);

        if (input == "/quit") {
            network.Disconnect();
            running = false;
            break;
        }

        if (input == "/disconnect") {
            network.Disconnect();
            continue;
        }

        if (!input.empty()) {
            network.Send(input);
        }
    }

    network.Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

network.Shutdown();
slow_cout << "Goodbye!" << std::endl;
return 0;

}



slow_cout << "=== Game Client with Heartbeat ===" << std::endl;

auto& network = NetworkManager::GetInstance();

if (!network.Initialize()) {
    std::cerr << "Failed to initialize network" << std::endl;
    return 1;
*/
