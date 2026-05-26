#pragma once
#include <string>

class QuestStage {
 public:
  QuestStage(const std::string& description, bool isDone = false);
  ~QuestStage() = default;

 private:
  std::string description;
  bool is_done;
};
