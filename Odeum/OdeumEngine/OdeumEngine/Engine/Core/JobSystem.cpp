#include "JobSystem.h"

#include "../DataStructures/RingBuffer.h"

#include <algorithm>

std::unique_ptr<JobSystem> JobSystem::sm_jobSystemInstance = nullptr;

void JobSystem::Initialize()
{
	m_finishedJobs.store(0);

	auto numCores = std::thread::hardware_concurrency();

	m_numThreads = std::max(1u, numCores);

    for (uint32_t threadID = 0; threadID < m_numThreads; threadID++)
    {

        m_workers.push_back(ThreadHandle());
        m_workers[threadID].thread = std::thread([] 
        {
            while (true)
            {
                if (JobSystem::Get().)
            }
        });


        std::thread worker([] {

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

        });

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
	while (IsBusy) { Poll(); }
}

void JobSystem::Poll()
{
	m_fence.notify_one();
	std::this_thread::yield();
}
