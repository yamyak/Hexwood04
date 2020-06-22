#pragma once

#include "Object.h"

#include <atomic>

class Colony;

class Ship : public Object
{
public:
	Ship(Colony* colony, float speed, bool interstellar, Object* obj);
	~Ship();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	Colony* GetSourceColony();

private:
	float m_x;
	float m_y;
	float m_z;
	float m_speed;
	Colony* m_source;
	bool m_interstellar;
	Object* m_destination;
	bool m_started;
	static std::atomic<int> m_global_id;
};

