#include "Dialogue.h"
Dialogue::Dialogue(const std::string& id) : id(id) {};
void Dialogue::addNode(const std::string& nodeId, DialogueNode* node) {
  nodes_di[nodeId] = node;
}

void Dialogue::setStartNode(const std::string& nodeId) { startNodeId = nodeId; }

DialogueNode* Dialogue::getStartNode() const {
  auto it = nodes_di.find(startNodeId);
  return it != nodes_di.end() ? it->second : nullptr;
}

DialogueNode* Dialogue::getNode(const std::string& nodeId) const {
  auto it = nodes_di.find(nodeId);
  return it != nodes_di.end() ? it->second : nullptr;
}

void Dialogue::chooseChoice(const std::string& choiceId) {
  // Реализация перехода по выбору
}

const std::string& Dialogue::getId() const { return id; }
