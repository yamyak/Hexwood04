#include "TimeEngine.h"


TimeEngine::TimeEngine(Object* obj) : m_process_count(0), m_base(obj)
{

}

TimeEngine::~TimeEngine()
{

}

void TimeEngine::Start(int thread_count)
{
	m_queue.push(m_base);

	std::vector<std::thread> threads;
	for (int i = 0; i < thread_count; i++)
	{
		threads.push_back(std::thread(&TimeEngine::Start_Thread, std::ref(m_queue_mutex), std::ref(m_process_count), std::ref(m_queue)));
	}

	for (auto& thr : threads)
	{
		thr.join();
	}
}

void TimeEngine::Start_Thread(std::mutex& mutex, std::atomic<int>& count, std::queue<Object*>& queue)
{
	while (!queue.empty() || count > 0)
	{
		if (count > 0)
		{
			count++;

			std::unique_lock<std::mutex> lock(mutex);
			Object* obj = queue.front();
			queue.pop();
			lock.unlock();

			obj->Run(mutex, queue);

			count--;
		}
	}
}