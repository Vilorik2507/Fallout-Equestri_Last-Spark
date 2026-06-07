#include "Player.h"

#include <iostream>

void Player::modifyReputation(const Faction& faction, int delta) {
  reputation_pl[faction] += delta;
}

void Player::addItem(std::shared_ptr<Item> item) {
  inventory_pl.addItem(item);
  std::cout << "Получен предмет: " << item->getName();
}
int Player::getGold() const { return gold; }
void Player::setGold(int amount) { gold = amount; }
void Player::spendGold(int amount) {
  if (amount <= gold) gold -= amount;
}
void Player::addGold(int amount) { gold += amount; }

Inventory& Player::getInventory() { return inventory_pl; }
const Inventory& Player::getInventory() const { return inventory_pl; }

void Player::healPlayer(int amount) {
  hp_pl += amount;
  if (hp_pl > max_hp_pl) hp_pl = max_hp_pl;
  std::cout << name_pl << " healed by " << amount << ". HP: " << hp_pl << "/"
            << max_hp_pl << std::endl;
}

void Player::useItem(int index) {
  auto items = inventory_pl.getItems();
  if (index < 1 || index > (int)items.size()) {
    std::cout << "Invalid item index.\n";
    return;
  }
  auto item = items[index - 1];
  item->use(this);
  inventory_pl.removeItem(item);
}

void Player::applyDamage(int damage) {
  hp_pl -= damage;
  if (hp_pl < 0) hp_pl = 0;
  std::cout << name_pl << " health: " << hp_pl << "/" << max_hp_pl << std::endl;
}
