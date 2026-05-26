#pragma once
#include <map>
#include <string>

#include "Faction.h"
#include "Quest.h"

class Choice {
 public:
  Choice(const std::string& text, const std::map<Faction*, int>& repDelta,
         const std::string& nextNodeId, Quest* quest);
  ~Choice() = default;

  const std::string& getText() const { return text_ch; };
  const std::map<Faction*, int>& getReputationDelta() const {
    return rep_delta;
  };
  const std::string& getNextNodeId() const { return next_node_id; };
  Quest* getTriggeredQuest() const { return triggered_quest; };

 private:
  std::string text_ch;
  std::map<Faction*, int> rep_delta;
  std::string next_node_id;
  Quest* triggered_quest;
};
