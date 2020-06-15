#include "Ship.h"

Ship::Ship()
{

}

Ship::~Ship()
{

}

void Ship::Run(std::mutex& mutex, std::queue<Object*>& queue)
{

}

int Ship::GetEmpireId()
{
	return m_empire_id;
}

void Ship::SetEmpireId(int id)
{
	m_empire_id = id;
}