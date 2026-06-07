#include "Enemy.h"

#include "ResourceManager.h"

Enemy::Enemy(const std::string& name, int level, int hp, int damage,
             int initiative, const std::string& loot_table_id)
    : name_en(name),
      level_en(level),
      hp_en(hp),
      max_hp_en(hp),
      damage_en(damage),
      initiative_en(initiative),
      ai_en(nullptr),
      loot_table_en(loot_table_id),
      distance_en(5) {}

void Enemy::applyDamage(int damage) {
  hp_en -= damage;
  if (hp_en < 0) hp_en = 0;
  std::cout << name_en << " health: " << hp_en << "/" << max_hp_en << std::endl;
}

int Enemy::getHp() const { return hp_en; }
bool Enemy::isAlive() const { return hp_en > 0; }
int Enemy::getInitiative() const { return initiative_en; }
const std::string& Enemy::getName() const { return name_en; }
int Enemy::getLevel() const { return level_en; }
int Enemy::getDamage() const { return damage_en; }
std::string& Enemy::getLootTableId() { return loot_table_en; }

void Enemy::setDistance(int distance) { distance_en = distance; }
int Enemy::getDistance() const { return distance_en; }

void Enemy::dropReward() { ResourceManager::generateLoot(loot_table_en); }
