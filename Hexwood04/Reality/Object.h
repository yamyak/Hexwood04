#pragma once

#include <mutex>
#include <queue>

class Object
{
public:
	Object();
	~Object();

	virtual bool Run(std::mutex& mutex, std::queue<Object*>& queue) = 0;
	int GetId();
	void SetId(int id);
	void Lock();
	void Unlock();

private:
	int m_id;
	std::mutex m_object_mutex;
	std::unique_lock<std::mutex> m_lock;
};

