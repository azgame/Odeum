#include "JobSystem.h"

#include <algorithm>

std::unique_ptr<JobSystem> JobSystem::sm_jobSystemInstance = nullptr;

void JobSystem::Initialize()
{
	m_finishedJobs.store(0);

	auto numCores = std::thread::hardware_concurrency();

	m_numThreads = std::max(1u, numCores - 1);

    for (uint32_t threadID = 0; threadID < m_numThreads; threadID++)
    {
        std::thread worker(&JobSystem::ThreadInitialize, this);
        worker.detach();
    }
}

void JobSystem::Execute(const std::function<void()>& Job)
{
	m_jobCount++;

	while (!m_jobQueue.push_back(Job)) Poll();

	m_fence.notify_one();
}

bool JobSystem::IsBusy()
{
	return m_finishedJobs.load() < m_jobCount;
}

void JobSystem::WaitForCompletion()
{
	while (IsBusy()) { Poll(); }
}

void JobSystem::Poll()
{
	m_fence.notify_one();
	std::this_thread::yield();
}

void JobSystem::ThreadInitialize()
{
    std::function<void()> job;

    while (true)
    {
        if (m_jobQueue.pop_front(job))
        {
            job();
            m_finishedJobs.fetch_add(1);
        }
        else
        {
            std::unique_lock<std::mutex> lock(sm_mutex);
            m_fence.wait(lock);
        }
    }
}
