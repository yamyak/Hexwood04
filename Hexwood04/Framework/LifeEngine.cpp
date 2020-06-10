#include "LifeEngine.h"
#include "../Utilities/ConfigReader.h"


LifeEngine::LifeEngine(int seed)
{
	m_generator.seed(seed);
	m_uniform_dist.param(std::uniform_int_distribution<int>::param_type(0, std::numeric_limits<int>::max()));
	m_normal_dist.param(std::normal_distribution<float>::param_type(ConfigReader::GetInstance()->GetFloat(Constants::EMPIRE, Constants::AVERAGE_EMPIRES), 
		ConfigReader::GetInstance()->GetFloat(Constants::EMPIRE, Constants::EMPIRE_SIGMA)));
}

LifeEngine::~LifeEngine()
{

}

bool LifeEngine::CreateEmpires(Universe& verse)
{
	int number_empires = (int)m_normal_dist(m_generator);
	int number_stars = verse.GetSize();
	int number_planets = 0;
	int index = 0;

	for (int i = 0; i < number_empires; i++)
	{
		bool planet_found = false;
		while (!planet_found)
		{
			index = m_uniform_dist(m_generator) % number_stars;
			Star* star = verse.GetStar(index);

			number_planets = star->GetSystemSize();
			if (number_planets > 0)
			{
				index = m_uniform_dist(m_generator) % number_planets;
				Planet* planet = star->GetPlanet(index);

				if (planet->SetOccupied())
				{
					Empire* empire = new Empire(i);
					Colony* colony = new Colony(planet);
					verse.AddColony(colony);
					empire->AddColony(colony);

					verse.AddEmpire(empire);

					planet_found = true;
				}
			}
		}
	}
}