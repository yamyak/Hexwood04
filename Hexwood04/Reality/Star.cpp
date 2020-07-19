#include "Star.h"
#include "Planet.h"


/// <summary>
/// Constructor
/// </summary>
/// <param name="id">Star id</param>
/// <param name="name">Star name</param>
/// <param name="x">Star x location in km from Sol</param>
/// <param name="y">Star y location in km from Sol</param>
/// <param name="z">Star z location in km from Sol</param>
/// <returns></returns>
Star::Star(int id, std::string& name, float x, float y, float z) : m_name(name), m_x(x), m_y(y), m_z(z)
{
	SetId(id);
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Star::~Star()
{

}

/// <summary>
/// Add reference to planet to star's system
/// </summary>
/// <param name="planet">Planet pointer</param>
void Star::AddPlanet(Planet* planet)
{
	m_planets[planet->GetId()] = planet;
}

/// <summary>
/// Star processing method
/// </summary>
/// <param name="mutex">Processing queue mutex</param>
/// <param name="queue">Processing queue</param>
void Star::Run(std::mutex& mutex, std::queue<Object*>& queue)
{

}

/// <summary>
/// Return number of planets in system
/// </summary>
/// <returns>Number of planets in system</returns>
int Star::GetSystemSize()
{
	return (int)m_planets.size();
}

/// <summary>
/// Returns planet associated with key provided
/// </summary>
/// <param name="key">Planet id</param>
/// <returns>Planet pointer</returns>
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

/// <summary>
/// Return location of star
/// </summary>
/// <returns>Tuple of x, y, z location of star</returns>
std::tuple<float, float, float> Star::GetLocation()
{
	return std::make_tuple(m_x, m_y, m_z);
}

/// <summary>
/// Returns list of all planets in system
/// </summary>
/// <returns>List of all planets in system</returns>
std::vector<Planet*> Star::GetSystem()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::vector<Planet*> planets;

	// copy all planet pointers into return vector
	for (auto& pl : m_planets)
	{
		planets.push_back(pl.second);
	}

	return planets;
}