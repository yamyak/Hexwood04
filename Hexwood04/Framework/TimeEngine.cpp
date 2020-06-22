#include "TimeEngine.h"
#include "../Reality/Object.h"
#include "../Reality/Universe.h"
#include "../Constants.h"
#include "../Utilities/Logger.h"
#include "../Utilities/ConfigReader.h"

#include <chrono>
#include <iostream>
#include <fstream>


using namespace Constants;

TimeEngine::TimeEngine() : m_process_count(0), m_run(true)
{

}

TimeEngine::~TimeEngine()
{

}

void TimeEngine::Start(int thread_count)
{
	std::vector<Object*> objects = Universe::GetInstance()->GetObjects(ObjectType::EMPIRE);

	Logger::GetInstance()->OpenFile(ConfigReader::GetInstance()->Get(Constants::INITIALIZATION, Constants::LOGGING_FILE));

	std::thread user_input(std::thread(&TimeEngine::User_Input_Thread, std::ref(m_run)));

	while(m_run)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (Object* obj : objects)
		{
			std::lock_guard<std::mutex> queue_lock(m_queue_mutex);
			m_queue.push(obj);
		}

		Universe::GetInstance()->IncrementAge();

		std::vector<std::thread> threads;
		for (int i = 0; i < thread_count; i++)
		{
			threads.push_back(std::thread(&TimeEngine::Start_Thread, std::ref(m_queue_mutex), std::ref(m_process_count), std::ref(m_queue)));
		}

		for (auto& thr : threads)
		{
			thr.join();
		}

		int age = Universe::GetInstance()->GetAge();

		Universe::GetInstance()->ClearOutGraveyard();

		auto stop = std::chrono::high_resolution_clock::now();
		Logger::GetInstance()->RecordData(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
	}

	user_input.join();

	Logger::GetInstance()->CloseFile();
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

void TimeEngine::User_Input_Thread(std::atomic<bool>& run)
{
	std::string s;

	while (run)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::cin >> s;
		if (s == "x")
		{
			run = false;
		}
	}
}