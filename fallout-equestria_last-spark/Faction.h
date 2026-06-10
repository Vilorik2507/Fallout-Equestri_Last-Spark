#ifndef FACTION_H
#define FACTION_H
#include <string>

class Faction {
public:
    Faction(int id, const std::string& name);
    ~Faction() = default;
    bool operator<(const Faction& other) const {
        return id_fac < other.id_fac;  // сортировка по уникальному идентификатору
    }

private:
    int id_fac;
    std::string name_fac;
};
#endif // !FACTION_H

