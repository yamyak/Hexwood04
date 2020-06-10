#pragma once

#include <mutex>

class Object
{
public:
	Object();
	~Object();

	virtual bool Run() = 0;
	int GetId();
	void SetId(int id);
	void Lock();
	void Unlock();

private:
	int m_id;
	std::mutex m_object_mutex;
	std::unique_lock<std::mutex> m_lock;
};

