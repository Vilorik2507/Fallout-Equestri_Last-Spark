#ifndef COMBAT_ACTION_H
#define COMBAT_ACTION_H
#include <string>
#include <vector>

#include "Combatant.h"
#include "Effect.h"

class CombatAction {
public:
	CombatAction(Combatant* source, Combatant* target, const std::string& type);

	bool calculateHit() const;
	void apply() const;

	void addEffect(const Effect& effect);

	Combatant* getSource() const { return source; }
	Combatant* getTarget() const { return target; }
	std::string getType() const { return type; }

private:
	Combatant* source;
	Combatant* target;
	std::string type;
	std::vector<Effect> effects;
};
#endif  // !COMBAT_ACTION_H

