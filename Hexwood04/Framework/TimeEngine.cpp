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

/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
TimeEngine::TimeEngine() : m_process_count(0), m_run(true)
{

}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
TimeEngine::~TimeEngine()
{

}

/// <summary>
/// Start the simulation
/// </summary>
/// <param name="thread_count">Number of threads to run on</param>
void TimeEngine::Start(int thread_count)
{
	// get all empire objects in the universe
	std::vector<Object*> objects = Universe::GetInstance()->GetObjects(ObjectType::EMPIRE);

	// open logging file
	Logger::GetInstance()->OpenFile(ConfigReader::GetInstance()->Get(Constants::INITIALIZATION, Constants::LOGGING_FILE));

	// start the user input thread
	std::thread user_input(std::thread(&TimeEngine::User_Input_Thread, std::ref(m_run)));

	// continue running while run flag is set
	while(m_run)
	{
		// initialize frame timing
		auto start = std::chrono::high_resolution_clock::now();

		// add all empires into processing queue
		for (Object* obj : objects)
		{
			std::lock_guard<std::mutex> queue_lock(m_queue_mutex);
			m_queue.push(obj);
		}

		// increment age of universe
		Universe::GetInstance()->IncrementAge();

		// start processing threads
		std::vector<std::thread> threads;
		for (int i = 0; i < thread_count; i++)
		{
			threads.push_back(std::thread(&TimeEngine::Start_Thread, std::ref(m_queue_mutex), std::ref(m_process_count), std::ref(m_queue)));
		}

		// wait for all processing threads to finish execution
		for (auto& thr : threads)
		{
			thr.join();
		}

		// clean out all dead objects
		Universe::GetInstance()->ClearOutGraveyard();

		// record frame time
		auto stop = std::chrono::high_resolution_clock::now();
		// log frame status
		Logger::GetInstance()->RecordData(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
	}

	// wait for user input thread to finish execution
	user_input.join();

	Logger::GetInstance()->CloseFile();
}

/// <summary>
/// Process universe objects
/// </summary>
/// <param name="mutex">Reference to processing queue mutex</param>
/// <param name="count">Reference to number of objects currently being processed</param>
/// <param name="queue">Reference to processing queue</param>
void TimeEngine::Start_Thread(std::mutex& mutex, std::atomic<int>& count, std::queue<Object*>& queue)
{
	// continue processing while the queue has elements and other threads are processing
	// if other threads are processing, they may add objects to the queue
	while (!queue.empty() || count > 0)
	{
		// increment number of objects being processed
		count++;

		// lock the queue and retrieve an object
		// then unlock queue
		std::unique_lock<std::mutex> lock(mutex);
		Object* obj = queue.front();
		queue.pop();
		lock.unlock();

		// process the object
		obj->Run(mutex, queue);

		// decrement the number objects being processed
		count--;
	}
}

/// <summary>
/// Wait for user to manually cancel the simulation
/// User needs to enter the "x" key
/// </summary>
/// <param name="run">Reference to processing flag</param>
void TimeEngine::User_Input_Thread(std::atomic<bool>& run)
{
	std::string s;

	// continue checking while run flag is still set
	while (run)
	{
		// sleep 1 second between each check
		std::this_thread::sleep_for(std::chrono::seconds(1));

		// wait for user input
		std::cin >> s;
		// clear the run flag is user provided needed input
		if (s == "x")
		{
			run = false;
		}
	}
}