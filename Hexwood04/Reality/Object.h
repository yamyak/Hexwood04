#pragma once

#include <mutex>
#include <queue>

class Object
{
public:
	Object();
	~Object();

	virtual void Run(std::mutex& mutex, std::queue<Object*>& queue) = 0;
	int GetId();
	void SetId(int id);

protected:
	std::mutex m_object_mutex;

private:
	int m_id;
};

