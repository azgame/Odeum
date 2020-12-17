#include "JobSystem.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <thread>

#include "../DataStructures/RingBuffer.h"

namespace JobSystem
{
    std::mutex sm_mutex;
    size_t m_numThreads;
    std::condition_variable m_fence;
    uint32_t m_jobCount;
    std::atomic<uint64_t> m_finishedJobs;
    RingBuffer<std::function<void()>> m_jobQueue(256);

    bool Work();
    void ThreadInitialize();
}

void JobSystem::Initialize()
{
	m_finishedJobs.store(0);

	auto numCores = std::thread::hardware_concurrency();

	m_numThreads = std::max(1u, numCores - 1);

    for (uint32_t threadID = 0; threadID < m_numThreads; threadID++)
    {
        std::thread worker(ThreadInitialize);
        worker.detach();
    }
}

void JobSystem::Execute(const std::function<void()>& Job)
{
	m_jobCount++;

    while (!m_jobQueue.push_back(Job)) m_fence.notify_all();

	m_fence.notify_one();
}

bool JobSystem::IsBusy()
{
	return m_finishedJobs.load() < m_jobCount;
}

void JobSystem::WaitForCompletion()
{
    m_fence.notify_all();
	while (IsBusy()) { Work(); }
}

bool JobSystem::Work()
{
    std::function<void()> job;

    if (m_jobQueue.pop_front(job))
    {
        job();
        m_finishedJobs.fetch_add(1);
        return true;
    }

    return false;
}

void JobSystem::ThreadInitialize()
{
    std::function<void()> job;

    while (true)
    {
        if (!Work())
        {
            std::unique_lock<std::mutex> lock(sm_mutex);
            m_fence.wait(lock);
        }
    }
}
