#ifndef CHARACTER_STATS_H
#define CHARACTER_STATS_H
#include <string>

class CharacterStats {
public:
	CharacterStats() = default;
	~CharacterStats() = default;

private:
	int strength_ch;
	int perception_ch;
	int endurance_ch;
	int charisma_ch;
	int intelligence_ch;
	int luck_ch;
};
#endif // !CHARACTER_STATS_H


