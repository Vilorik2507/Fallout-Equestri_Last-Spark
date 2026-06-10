#ifndef MOD_H
#define MOD_H
#include <map>
#include <string>

class Mod {
public:
	Mod(std::map<std::string, int>&);
	~Mod() = default;

	const std::map<std::string, int>& getBonuses() const;
	const std::string& getName() const;

private:
	std::string name_mod;
	std::map<std::string, int> stat_bonus;
};
#endif // !MOD_H

