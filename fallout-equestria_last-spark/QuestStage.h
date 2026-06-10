#ifndef QUEST_STAGE_H
#define QUEST_STAGE_H
#include <string>

class QuestStage {
public:
	QuestStage(const std::string& description, bool isDone = false);
	~QuestStage() = default;

private:
	std::string description;
	bool is_done;
};
#endif // !QUEST_STAGE_H

