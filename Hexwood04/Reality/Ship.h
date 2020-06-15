#pragma once

#include "Object.h"

class Planet;
class Star;

class Ship : public Object
{
public:
	Ship();
	~Ship();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	void SetEmpireId(int id);
	int GetEmpireId();

private:
	int m_empire_id;
	float m_x;
	float m_y;
	float m_z;
	float m_speed;
	Planet* m_source;
	bool m_interstellar;
	Star* m_star_destination;
	Planet* m_planet_destination;
};

