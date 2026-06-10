#ifndef DIALOGUE_NODE_H
#define DIALOGUE_NODE_H
#include <string>
#include <vector>

#include "Choice.h"

class DialogueNode {
public:
	DialogueNode(const std::string& npcLine, const std::vector<Choice>& choices);
	~DialogueNode() = default;

	const std::string& getNpcLine() const { return npc_line; };
	const std::vector<Choice>& getChoices() const { return choices_pl; };

private:
	std::string npc_line;
	std::vector<Choice> choices_pl;
};
#endif // !DIALOGUE_NODE_H

