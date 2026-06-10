#ifndef QUEST_H
#define QUEST_H
#include <string>
#include <vector>

#include "QuestStage.h"

class Quest {
public:
	Quest(const std::string& name, const std::vector<QuestStage*>& stages);
	~Quest() = default;


private:
	std::string name_quest;
	std::vector<QuestStage*> stages_quest;
	int current_stage;
};
#endif // !QUEST_H

