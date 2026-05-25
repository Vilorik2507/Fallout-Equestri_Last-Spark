#pragma once
#include <string>

class QuestStage {
 public:
  QuestStage() = default;
  ~QuestStage() = default;

 private:
  std::string description;
  bool is_done;
};
