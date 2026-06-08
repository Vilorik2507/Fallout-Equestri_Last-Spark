#pragma once
#include <string>

#include "ActionType.h"

class GameAction {
 public:
  GameAction() = default;
  ~GameAction() = default;

  std::string serialize();
  void deserialize(const std::string& data);

 private:
  ActionType type;
  long long timestamp;     // временная метка
  float position;          // позиция
  float direction;         // направление
  int damage;              // урон
  std::string extra_data;  // дополнительные данные
};
