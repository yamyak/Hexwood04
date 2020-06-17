#pragma once

#include <queue>
#include <mutex>
#include <atomic>

class Object;

class TimeEngine
{
public:
	TimeEngine();
	~TimeEngine();

	void Start(int thread_count);
	static void Start_Thread(std::mutex& mutex, std::atomic<int>& count, std::queue<Object*>& queue);

private:
	std::mutex m_queue_mutex;
	std::atomic<int> m_process_count;
	std::queue<Object*> m_queue;
};

