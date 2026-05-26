#include "Enemy.h"
Enemy::Enemy(const std::string& name, int level, int hp, int damage,
             int initiative)
    : name_en(name),
      level_en(level),
      hp_en(hp),
      damage_en(damage),
      initiative_en(initiative),
      max_hp_en(hp) {};
