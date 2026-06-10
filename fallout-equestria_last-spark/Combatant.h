
#ifndef COMBATANT_H
#define COMBATANT_H
#include <string>
class Combatant {
public:
	virtual ~Combatant() = default;

	virtual int getHp() const = 0;
	virtual void applyDamage(int damage) = 0;
	virtual int getDamage() const = 0;
	virtual bool isAlive() const = 0;
	virtual int getInitiative() const = 0;
	virtual const std::string& getName() const = 0;
};
#endif // !COMBATANT_H



