#ifndef GAME_ACTION_H
#define GAME_ACTION_H
#include <string>

#include "ActionType.h"

class GameAction {
public:
	GameAction() = default;
	~GameAction() = default;

private:
	ActionType type;
	long long timestamp;     // временная метка
	float position;          // позиция
	float direction;         // направление
	int damage;              // урон
	std::string extra_data;  // дополнительные данные
};
#endif  // !GAME_ACTION_H

