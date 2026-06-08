#include "Quest.h"
Quest::Quest(const std::string& name, const std::vector<QuestStage*>& stages)
    : name_quest(name), stages_quest(stages) {};
