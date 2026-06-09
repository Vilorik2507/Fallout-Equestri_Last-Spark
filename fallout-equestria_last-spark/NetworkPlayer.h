#pragma once
#include <string>

#include "Combatant.h"

class NetworkPlayer : public Combatant {
 public:
  NetworkPlayer(const std::string& nickname, int max_hp = 100,
                int initiative = 10);
  ~NetworkPlayer();

  void applyDamage(int damage) override;
  int getHp() const override;
  bool isAlive() const override;
  int getInitiative() const override;
  const std::string& getName() const override;
  int getDamage() override { return 1; };

  int getDistance() const { return m_distance; };
  void setDistance(int dist) { m_distance = dist; }
  void setName(std::string& name) { m_name = name; };

  void setHp(int hp);
  void setInitiative(int initiative);

 private:
  int m_distance = 5;
  std::string m_name;
  int m_hp;
  int m_max_hp;
  int m_initiative;
};
