#pragma once
#include <map>
#include <string>

#include "DialogueNode.h"

class Dialogue {
 public:
  Dialogue(const std::string& id);
  ~Dialogue() = default;

  void addNode(const std::string& nodeId, DialogueNode* node);
  DialogueNode* getStartNode();
  void chooseChoice(const std::string& choiceId);

 private:
  std::string id;
  std::map<std::string, DialogueNode*> nodes_di;
};
