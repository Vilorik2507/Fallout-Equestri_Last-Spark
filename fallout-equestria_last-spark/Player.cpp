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
