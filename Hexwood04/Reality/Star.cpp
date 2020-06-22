#include "Star.h"
#include "Planet.h"


Star::Star(int id, std::string& name, float x, float y, float z) : m_name(name), m_x(x), m_y(y), m_z(z)
{
	SetId(id);
}

Star::~Star()
{

}

void Star::AddPlanet(Planet* planet)
{
	m_planets[planet->GetId()] = planet;
}

void Star::Run(std::mutex& mutex, std::queue<Object*>& queue)
{

}

int Star::GetSystemSize()
{
	return (int)m_planets.size();
}

Planet* Star::GetPlanet(int key)
{
	int i = 0;
	for (auto& planet : m_planets)
	{
		if (i == key)
		{
			return planet.second;
		}

		i++;
	}
	
	return nullptr;
}

std::tuple<float, float, float> Star::GetLocation()
{
	return std::make_tuple(m_x, m_y, m_z);
}

std::vector<Planet*> Star::GetSystem()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::vector<Planet*> planets;

	for (auto& pl : m_planets)
	{
		planets.push_back(pl.second);
	}

	return planets;
}