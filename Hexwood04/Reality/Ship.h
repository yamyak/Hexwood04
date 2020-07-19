#pragma once

#include "Object.h"

#include <atomic>

class Colony;

/// <summary>
/// Ship object
/// </summary>
class Ship : public Object
{
public:
	Ship(Colony* colony, float speed, bool interstellar, Object* obj);
	~Ship();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	Colony* GetSourceColony();

private:
	// current ship location
	float m_x;
	float m_y;
	float m_z;
	// ship speed
	float m_speed;
	// host colony
	Colony* m_source;
	// whether ship is going interstellar or not
	bool m_interstellar;
	Object* m_destination;
	// 1 time frame flag for local journeys
	bool m_started;
	static std::atomic<int> m_global_id;
};

