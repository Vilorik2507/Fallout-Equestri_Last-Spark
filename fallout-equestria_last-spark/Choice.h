#pragma once
#include <map>
#include <string>

#include "Faction.h"
#include "Quest.h"

class Choice {
 public:
  Choice() = default;
  ~Choice() = default;

 private:
  std::string text_ch;
  std::map<Faction*, int> rep_delta;
  std::string next_node_id;
  Quest* triggered_quest;
};
