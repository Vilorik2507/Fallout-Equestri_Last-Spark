#ifndef PERK_H
#define PERK_H
#include <string>

class Perk {
public:
	Perk() = default;
	~Perk() = default;

private:
	std::string name_per;
	std::string effect_per;
};
#endif // !PERK_H

