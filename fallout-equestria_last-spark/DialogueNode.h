#pragma once
#include <string>
#include <vector>

#include "Choice.h"

class DialogueNode {
 public:
  DialogueNode() = default;
  ~DialogueNode() = default;

 private:
  std::string npc_line;
  std::vector<Choice> choices_pl;
};
