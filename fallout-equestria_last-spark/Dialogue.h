
#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <map>
#include <string>

#include "DialogueNode.h"

class Dialogue {
public:
	Dialogue(const std::string& id);
	~Dialogue();

	void addNode(const std::string& nodeId, DialogueNode* node);
	void setStartNode(const std::string& nodeId);
	DialogueNode* getStartNode() const;
	DialogueNode* getNode(const std::string& nodeId) const;
	const std::string& getId() const;

private:
	std::string id;
	std::map<std::string, DialogueNode*> nodes_di;
	std::string startNodeId;
};
#endif // !DIALOGUE_H

