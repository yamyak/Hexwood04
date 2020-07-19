#pragma once

#include <mutex>
#include <queue>

/// <summary>
/// Basic universe object
/// </summary>
class Object
{
public:
	Object();
	~Object();

	// virtual method that will be used to process the object
	virtual void Run(std::mutex& mutex, std::queue<Object*>& queue) = 0;
	int GetId();
	void SetId(int id);

protected:
	// object mutex to limit simultaneous processing of object
	std::mutex m_object_mutex;

private:
	int m_id;
};

