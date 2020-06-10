#pragma once

#include "../Reality/Object.h"

#include <queue>
#include <mutex>

class TimeEngine
{
public:
	TimeEngine(Object* obj);
	~TimeEngine();

	void IncProcessCount();
	void DecProcessCount();

	void StartSimulation();

private:
	std::mutex m_mutex;
	int m_process_count;
	std::queue<Object*> m_queue;
	Object* m_base;

	void ProcessFrame();
};

