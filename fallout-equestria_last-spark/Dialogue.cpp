#include "Dialogue.h"
void Dialogue::setStartNode(const std::string& nodeId) { startNodeId = nodeId; }

Dialogue::Dialogue(const std::string& id) : id(id) {};
void Dialogue::addNode(const std::string& nodeId, DialogueNode* node) {
  nodes_di[nodeId] = node;
}

DialogueNode* Dialogue::getStartNode() const {
  auto it = nodes_di.find(startNodeId);
  return it != nodes_di.end() ? it->second : nullptr;
}

DialogueNode* Dialogue::getNode(const std::string& nodeId) const {
  auto it = nodes_di.find(nodeId);
  return it != nodes_di.end() ? it->second : nullptr;
}

const std::string& Dialogue::getId() const { return id; }
Dialogue::~Dialogue() {
  for (auto& pair : nodes_di) {
    delete pair.second;
  }
  nodes_di.clear();
}
