#include "Ship.h"
#include "Star.h"
#include "Planet.h"
#include "Colony.h"
#include "Universe.h"
#include "Empire.h"
#include "../Constants.h"

#include <tuple>
#include <cmath>


using namespace Constants;

std::atomic<int> Ship::m_global_id = 0;

/// <summary>
/// Constructor
/// </summary>
/// <param name="colony">Host colony pointer</param>
/// <param name="speed">Ship speed</param>
/// <param name="interstellar">Whether ship is going interstellar</param>
/// <param name="obj">Target object</param>
/// <returns></returns>
Ship::Ship(Colony* colony, float speed, bool interstellar, Object* obj) : m_source(colony),
	m_speed(speed), m_x(0), m_y(0), m_z(0), m_interstellar(interstellar), 
	m_destination(obj), m_started(false)
{
	SetId(m_global_id++);

	// if going interstellar, need to record starting location
	if (m_interstellar)
	{
		std::tie(m_x, m_y, m_z) = m_source->GetPlanet()->GetStar()->GetLocation();
	}
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Ship::~Ship()
{

}

/// <summary>
/// Ship processing method
/// </summary>
/// <param name="mutex">Processing queue mutex</param>
/// <param name="queue">Processing queue</param>
void Ship::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// if going interstellar
	if (m_interstellar)
	{
		// get destination location
		float x, y, z;
		std::tie(x, y, z) = static_cast<Star*>(m_destination)->GetLocation();

		// calculate distance to destination
		float x_total = x - m_x;
		float y_total = y - m_y;
		float z_total = z - m_z;
		float dist_total = std::sqrt(std::pow(x_total, 2) + std::pow(y_total, 2) + std::pow(z_total, 2));

		// calculate distance traveled
		float dist_delta = m_speed * SECONDS_PER_YEAR;

		// if still traveling
		if (dist_delta < dist_total)
		{
			// update location
			float x_delta = dist_delta * x_total / dist_total;
			float y_delta = dist_delta * y_total / dist_total;
			float z_delta = dist_delta * z_total / dist_total;

			m_x += x_delta;
			m_y += y_delta;
			m_z += z_delta;
		}
		// destination reached
		else
		{
			// get destination star
			Star* star = static_cast<Star*>(m_destination);
			std::vector<Planet*> system = star->GetSystem();
			Colony* colony = new Colony(m_source);
			bool found = false;
			// iterate through planets in system 
			for (Planet* planet : system)
			{
				// find unoccupied planet and colonize it
				if (planet->SetOccupied() && m_source->GetEmpire()->SetColonized(ObjectType::PLANET, planet->GetId(), colony->GetId()))
				{
					// add new colony to planet
					colony->SetPlanet(planet);
					Universe::GetInstance()->AddColony(colony);
					m_source->GetEmpire()->AddColony(colony);
					found = true;
					break;
				}
			}

			// if not available planets found, delete created colony
			if (!found)
			{
				delete colony;
				colony = nullptr;
			}

			// add ship to graveyard for cleanup, regardless of mission success or not
			Universe::GetInstance()->AddToGraveyard(ObjectType::SHIP, GetId());
		}
	}
	// local journey
	else
	{
		// wait 1 period for ship to reach destination
		if (m_started)
		{
			Planet* planet = static_cast<Planet*>(m_destination);
			// attempt to colonize planet
			if (planet->SetOccupied())
			{
				// create new colony and add to planet
				Colony* colony = new Colony(m_source);
				colony->SetPlanet(planet);
				Universe::GetInstance()->AddColony(colony);
				m_source->GetEmpire()->AddColony(colony);
			}

			// add ship to graveyard for cleanup, regardless of mission success or not
			Universe::GetInstance()->AddToGraveyard(ObjectType::SHIP, GetId());
		}

		m_started = true;
	}
}

/// <summary>
/// Return host colony
/// </summary>
/// <returns>Host colony pointer</returns>
Colony* Ship::GetSourceColony()
{
	return m_source;
}