#include "Choice.h"
Choice::Choice(const std::string& text, const std::map<Faction*, int>& repDelta,
               const std::string& nextNodeId, Quest* quest)
    : text_ch(text),
      rep_delta(repDelta),
      next_node_id(nextNodeId),
      triggered_quest(quest) {};
