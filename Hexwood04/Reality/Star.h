#pragma once

#include "Object.h"
#include "Planet.h"

#include <map>

class Star : public Object
{
public:
	inline Star() {};
	Star(int id, std::string& name, float x, float y, float z);
	~Star();

	void AddPlanet(Planet* planet);
	int GetSystemSize();
	Planet* GetPlanet(int key);
	bool Run();

private:
	float m_x;
	float m_y;
	float m_z;
	std::string m_name;
	std::map<int, Planet*> m_planets;
};

