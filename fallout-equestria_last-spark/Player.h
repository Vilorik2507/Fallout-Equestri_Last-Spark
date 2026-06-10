#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "CharacterStats.h"
#include "Combatant.h"
#include "Effect.h"
#include "Faction.h"
#include "Inventory.h"
#include "Item.h"
#include "Perk.h"
#include "Quest.h"
#include "Skill.h"
#include "Weapon.h"

class Player : public Combatant {
public:
    Player() = default;
    ~Player() = default;

    void createNew();
    void levelUp();
    void addExp(int amount);
    void addItem(std::shared_ptr<Item>);
    void addPerk(const Perk& perk);
    void modifyReputation(const Faction& faction, int delta);
    void healPlayer(int amount);
    void applyBuffEffect(const Effect& effect, int duration);
    Inventory& getInventory();
    const Inventory& getInventory() const;
    // Combatant interface
    int getHp() const override { return 100; };
    bool isAlive() const override { return hp_pl > 0; };
    int getInitiative() const override { return 10; };
    int getDamage() const override;

    std::shared_ptr<Weapon> getActivWeopon() { return activ_weopon; };
    void setActivWeopon(std::shared_ptr<Weapon> weopon) {
        activ_weopon = weopon;
    };
    void applyDamage(int damage) override;

    int getGold() const;
    void setGold(int amount);
    void spendGold(int amount);
    void addGold(int amount);
    const std::string& getName() const override { return name_pl; };
    void setName(std::string& name) { name_pl = name; };
    void useItem(int index);

private:
    std::shared_ptr<Weapon> activ_weopon;
    std::string name_pl = "Пони";
    int level_pl;
    int gold = 100;
    int exp_pl;
    CharacterStats stats_pl;
    std::vector<Skill> skills_pl;
    std::vector<Perk> perks_pl;
    Inventory inventory_pl;
    std::map<Faction, int> reputation_pl;
    int hp_pl = 100;
    int max_hp_pl = 100;
};
#endif // !PLAYER_H

