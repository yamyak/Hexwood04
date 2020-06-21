#include "TimeEngine.h"
#include "../Reality/Object.h"
#include "../Reality/Universe.h"
#include "../Constants.h"

#include <map>
#include <chrono>
#include <iostream>
#include <fstream>


using namespace Constants;

TimeEngine::TimeEngine() : m_process_count(0)
{

}

TimeEngine::~TimeEngine()
{

}

void TimeEngine::Start(int thread_count)
{
	std::vector<Object*> objects = Universe::GetInstance()->GetObjects(ObjectType::EMPIRE);

	std::ofstream myfile("timing.csv");
	if (myfile.is_open())
	{
		bool running = true;
		for(int i = 0; i < 50000; i++)
		{
			for (Object* obj : objects)
			{
				std::lock_guard<std::mutex> queue_lock(m_queue_mutex);
				m_queue.push(obj);
			}

			Universe::GetInstance()->IncrementAge();

			auto start = std::chrono::high_resolution_clock::now();

			std::vector<std::thread> threads;
			for (int i = 0; i < thread_count; i++)
			{
				threads.push_back(std::thread(&TimeEngine::Start_Thread, std::ref(m_queue_mutex), std::ref(m_process_count), std::ref(m_queue)));
			}

			for (auto& thr : threads)
			{
				thr.join();
			}

			Universe::GetInstance()->ClearOutGraveyard();

			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

			myfile << duration.count() << std::endl;
		}

		myfile.close();
	}
}

void TimeEngine::Start_Thread(std::mutex& mutex, std::atomic<int>& count, std::queue<Object*>& queue)
{
	while (!queue.empty() || count > 0)
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