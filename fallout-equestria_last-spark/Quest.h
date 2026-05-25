#pragma once
#include <string>
#include <vector>

#include "QuestStage.h"

class Quest {
 public:
  Quest() = default;
  ~Quest() = default;

  void advanceQuest();

 private:
  std::string name_quest;
  std::vector<QuestStage*> stages_quest;
  int current_stage;
};
