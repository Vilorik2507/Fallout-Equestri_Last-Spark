#include "DialogueNode.h"
DialogueNode::DialogueNode(const std::string& npcLine,
                           const std::vector<Choice>& choices)
    : npc_line(npcLine), choices_pl(choices) {};
