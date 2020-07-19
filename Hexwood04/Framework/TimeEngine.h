#pragma once

#include <queue>
#include <mutex>
#include <atomic>

class Object;

/// <summary>
/// Runs the simulations on threads 
/// </summary>
class TimeEngine
{
public:
	TimeEngine();
	~TimeEngine();

	void Start(int thread_count);
	static void Start_Thread(std::mutex& mutex, std::atomic<int>& count, std::queue<Object*>& queue);
	static void User_Input_Thread(std::atomic<bool>& run);

private:
	// access lock for the object queue
	std::mutex m_queue_mutex;
	// number of threads currently running
	std::atomic<int> m_process_count;
	// run flag
	std::atomic<bool> m_run;
	// queue of objects slated for processing
	std::queue<Object*> m_queue;
};

