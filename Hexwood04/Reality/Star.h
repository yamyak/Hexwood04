#pragma once

#include "Object.h"
#include "Planet.h"

#include <map>

class Star : public Object
{
public:
	inline Star() : m_x(0), m_y(0), m_z(0) {};
	Star(int id, std::string& name, float x, float y, float z);
	~Star();

	void AddPlanet(Planet* planet);
	int GetSystemSize();
	Planet* GetPlanet(int key);
	bool Run(std::mutex& mutex, std::queue<Object*>& queue);

private:
	float m_x;
	float m_y;
	float m_z;
	std::string m_name;
	std::map<int, Planet*> m_planets;
};

