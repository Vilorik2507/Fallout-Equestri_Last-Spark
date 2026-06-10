#ifndef SKILL_H
#define SKILL_H
#include <string>

class Skill {
public:
	Skill() = default;
	~Skill() = default;

private:
	std::string name_sk;
	int value_sk;
};
#endif // !SKILL_H

