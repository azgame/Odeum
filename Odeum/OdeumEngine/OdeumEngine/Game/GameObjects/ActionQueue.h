#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include "Observer.h"
#include "Action.h"

#include <queue>

/*	
	Send all game actions here, as a "middle man" in between player action/input and whatever processing
	needs to be done on it, eg. checking for proc effects


*/

class ActionQueue : public Subject
{
public:

	ActionQueue(const ActionQueue&) = delete;
	ActionQueue(ActionQueue&&) = delete;
	ActionQueue& operator=(const ActionQueue&) = delete;
	ActionQueue& operator=(ActionQueue&&) = delete;

	static ActionQueue* Get()
	{
		if (sm_instance.get() == nullptr)
			sm_instance.reset(new ActionQueue);
		return sm_instance.get();
	}

private:

	ActionQueue() {}
	~ActionQueue() {}

	static std::unique_ptr<ActionQueue> sm_instance;
	static std::queue<Action*> sm_actionQueue;
	friend std::default_delete<ActionQueue>;
	static std::mutex sm_mutex;

};

#endif