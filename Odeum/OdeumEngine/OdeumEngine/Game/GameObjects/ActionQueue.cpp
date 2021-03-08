#include "ActionQueue.h"

std::unique_ptr<ActionQueue> ActionQueue::sm_instance = nullptr;
std::queue<Action*> ActionQueue::sm_actionQueue;
