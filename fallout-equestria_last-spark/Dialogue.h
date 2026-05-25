#pragma once
#include <map>
#include <string>

#include "DialogueNode.h"

class Dialogue {
 public:
  Dialogue() = default;
  ~Dialogue() = default;

  DialogueNode* getStartNode();
  void chooseChoice(const std::string& choiceId);

 private:
  std::map<std::string, DialogueNode*> nodes_di;
};
