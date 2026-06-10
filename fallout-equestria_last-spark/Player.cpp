#include "Player.h"

#include <iostream>

#include "Consumable.h"
#include "Printer.h"

int Player::getGold() const { return gold; }
void Player::setGold(int amount) { gold = amount; }
void Player::addGold(int amount) { gold += amount; }
Inventory& Player::getInventory() { return inventory_pl; }
const Inventory& Player::getInventory() const { return inventory_pl; }

void Player::modifyReputation(const Faction& faction, int delta) {
  reputation_pl[faction] += delta;
}
void Player::spendGold(int amount) {
  if (amount <= gold) gold -= amount;
}
void Player::applyDamage(int damage) {
  hp_pl -= damage;
  if (hp_pl < 0) hp_pl = 0;
  slow_cout << name_pl << " health: " << hp_pl << "/" << max_hp_pl << std::endl;
}
void Player::addExp(int amount) {
  exp_pl += amount;
  while (exp_pl >= level_pl * 100) {
    levelUp();
  }
}
void Player::addItem(std::shared_ptr<Item> item) {
  inventory_pl.addItem(item);
  slow_cout << "Получен предмет: " << item->getName() << std::endl;
}

int Player::getDamage() const {
  if (activ_weopon)
    return activ_weopon->getDamage();
  else {
    return 5;
  }
}

void Player::healPlayer(int amount) {
  hp_pl += amount;
  if (hp_pl > max_hp_pl) hp_pl = max_hp_pl;
  slow_cout << name_pl << " Востановил жизни: " << amount << ". HP: " << hp_pl
            << "/" << max_hp_pl << std::endl;
}
void Player::levelUp() {
  level_pl++;
  max_hp_pl += 10;
  hp_pl = max_hp_pl;
  slow_cout << name_pl << " повысил уровень до " << level_pl
            << "! HP: " << hp_pl << "/" << max_hp_pl << std::endl;
}
void Player::useItem(int index) {
  auto items = inventory_pl.getItems();
  if (index < 1 || index > (int)items.size()) {
    slow_cout << "Invalid item index.\n";
    return;
  }
  auto item = items[index - 1];
  item->use(this);
  auto* consumable = dynamic_cast<Consumable*>(item.get());
  if (consumable) {
    inventory_pl.removeItem(item);
  } else {
    slow_cout << "Применён " << item->getName() << std::endl;
  }
}
