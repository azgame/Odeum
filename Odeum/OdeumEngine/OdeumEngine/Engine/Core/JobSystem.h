#ifndef JOBSYSTEM_H
#define JOBSYSTEM_H

#include "../../pch.h"
#include "../DataStructures/RingBuffer.h"

#include <functional>
#include <atomic>
#include <condition_variable>
#include <thread>

class JobSystem
{	
public:

	static JobSystem& Get() 
	{
		if (sm_jobSystemInstance.get() == nullptr)
			sm_jobSystemInstance.reset(new JobSystem);
		return *sm_jobSystemInstance.get(); 
	}

	void Initialize();

	void Execute(const std::function<void()>& Job);

	bool IsBusy();
	void WaitForCompletion();

private:

	JobSystem() : m_jobQueue(256) {}

	void Poll();
	void ThreadInitialize();

	static std::unique_ptr<JobSystem> sm_jobSystemInstance;
	friend std::default_delete<JobSystem>;

	std::mutex sm_mutex;
	size_t m_numThreads;
	std::condition_variable m_fence;
	uint32_t m_jobCount;
	std::atomic<uint64_t> m_finishedJobs;
	RingBuffer<std::function<void()>> m_jobQueue;
	
public:

	JobSystem(const JobSystem&) = delete;
	JobSystem(JobSystem&&) = delete;
	JobSystem& operator=(const JobSystem&) = delete;
	JobSystem& operator=(JobSystem&&) = delete;
};

#endif