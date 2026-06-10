#include "NetworkPlayer.h"

int NetworkPlayer::getHp() const { return m_hp; }
bool NetworkPlayer::isAlive() const { return m_hp > 0; }
int NetworkPlayer::getInitiative() const { return m_initiative; }
const std::string& NetworkPlayer::getName() const { return m_name; }
void NetworkPlayer::setHp(int hp) { m_hp = hp; }
void NetworkPlayer::setInitiative(int initiative) { m_initiative = initiative; }
NetworkPlayer::~NetworkPlayer() {}

NetworkPlayer::NetworkPlayer(const std::string& nickname, int max_hp,
                             int initiative)
    : m_name(nickname),
      m_hp(max_hp),
      m_max_hp(max_hp),
      m_initiative(initiative) {}

void NetworkPlayer::applyDamage(int damage) {
  m_hp -= damage;
  if (m_hp < 0) m_hp = 0;
}
