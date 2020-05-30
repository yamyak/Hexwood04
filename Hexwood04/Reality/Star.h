#pragma once

#include "Object.h"
#include "Planet.h"

#include <string>
#include <map>

class Star : public Object
{
public:
	Star();
	Star(int id, std::string& name, float x, float y, float z);
	~Star();

	int GetId();
	void AddPlanet(Planet& planet);

private:
	int m_id;
	float m_x;
	float m_y;
	float m_z;
	std::string m_name;
	std::map<int, Planet> m_planets;
};

