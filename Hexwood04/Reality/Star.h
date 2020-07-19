#pragma once

#include "Object.h"

#include <map>
#include <tuple>
#include <vector>

class Planet;

/// <summary>
/// Star object
/// </summary>
class Star : public Object
{
public:
	inline Star() : m_x(0), m_y(0), m_z(0) {};
	Star(int id, std::string& name, float x, float y, float z);
	~Star();

	void AddPlanet(Planet* planet);
	int GetSystemSize();
	Planet* GetPlanet(int key);
	std::vector<Planet*> GetSystem();
	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	std::tuple<float, float, float> GetLocation();

private:
	// Location of star
	float m_x;
	float m_y;
	float m_z;
	std::string m_name;
	// Map of planets in star's system
	std::map<int, Planet*> m_planets;
};

