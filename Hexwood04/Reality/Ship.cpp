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

Ship::Ship(Colony* colony, float speed, bool interstellar, Object* obj) : m_source(colony),
	m_speed(speed), m_x(0), m_y(0), m_z(0), m_interstellar(interstellar), 
	m_destination(obj), m_started(false)
{
	SetId(m_global_id++);

	if (m_interstellar)
	{
		std::tie(m_x, m_y, m_z) = m_source->GetPlanet()->GetStar()->GetLocation();
	}
}

Ship::~Ship()
{

}

void Ship::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	if (m_interstellar)
	{
		float x, y, z;
		std::tie(x, y, z) = static_cast<Star*>(m_destination)->GetLocation();

		float x_total = x - m_x;
		float y_total = y - m_y;
		float z_total = z - m_z;
		float dist_total = std::sqrt(std::pow(x_total, 2) + std::pow(y_total, 2) + std::pow(z_total, 2));

		float dist_delta = m_speed * SECONDS_PER_YEAR;

		if (dist_delta < dist_total)
		{
			float x_delta = dist_delta * x_total / dist_total;
			float y_delta = dist_delta * y_total / dist_total;
			float z_delta = dist_delta * z_total / dist_total;

			m_x += x_delta;
			m_y += y_delta;
			m_z += z_delta;
		}
		else
		{
			Star* star = static_cast<Star*>(m_destination);
			std::vector<Planet*> system = star->GetSystem();
			for (Planet* planet : system)
			{
				if (m_source->GetEmpire()->CheckOccupancy(ObjectType::PLANET, planet->GetId()) && planet->SetOccupied())
				{
					Colony* colony = new Colony(m_source);
					colony->SetPlanet(planet);
					Universe::GetInstance()->AddColony(colony);
					m_source->GetEmpire()->AddColony(colony);
					break;
				}
			}

			Universe::GetInstance()->AddToGraveyard(ObjectType::SHIP, GetId());
		}
	}
	else
	{
		if (m_started)
		{
			Planet* planet = static_cast<Planet*>(m_destination);
			if (planet->SetOccupied())
			{
				m_source->GetEmpire()->CleanOccupanyRecords(ObjectType::PLANET, planet->GetId());
				Colony* colony = new Colony(m_source);
				colony->SetPlanet(planet);
				Universe::GetInstance()->AddColony(colony);
				m_source->GetEmpire()->AddColony(colony);
			}

			Universe::GetInstance()->AddToGraveyard(ObjectType::SHIP, GetId());
		}

		m_started = true;
	}
}

Colony* Ship::GetSourceColony()
{
	return m_source;
}