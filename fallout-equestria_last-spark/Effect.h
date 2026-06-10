
#ifndef EFFECT_H
#define EFFECT_H
#include <map>
#include <string>

#include "Combatant.h"

class Effect {
public:
    Effect(const std::string& name, int duration,
        const std::map<std::string, int>& modifiers);
    ~Effect();

    void apply(Combatant* target) const;
    void tick(Combatant* target);

private:
    std::string name;
    int duration;
    std::map<std::string, int> modifiers;
};
#endif // !EFFECT_H

