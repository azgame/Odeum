#ifndef JOBSYSTEM_H
#define JOBSYSTEM_H

#include "../../pch.h"

#include <functional>

namespace JobSystem
{	
	void Initialize();

	void Execute(const std::function<void()>& Job);

	bool IsBusy();
	void WaitForCompletion();
};

#endif