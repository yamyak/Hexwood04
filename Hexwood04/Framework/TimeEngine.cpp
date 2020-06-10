#include "TimeEngine.h"


TimeEngine::TimeEngine(Object* obj) : m_base(obj), m_process_count(0)
{

}

TimeEngine::~TimeEngine()
{

}

void TimeEngine::IncProcessCount()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_process_count++;
}

void TimeEngine::DecProcessCount()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_process_count--;
}

void TimeEngine::StartSimulation()
{
	m_queue.push(m_base);

	while (true)
	{
		ProcessFrame();
	}
}

void TimeEngine::ProcessFrame()
{
	while (!m_queue.empty() || m_process_count > 0)
	{
		Object* obj = m_queue.front();
		obj->Run();
		m_queue.pop();
	}
}