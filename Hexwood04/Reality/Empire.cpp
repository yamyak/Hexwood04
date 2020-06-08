#include "Empire.h"

Empire::Empire(int id)
{
	SetId(id);
}

Empire::~Empire()
{

}

bool Empire::Run()
{
	return false;
}

void Empire::AddColony(Colony* colony)
{
	colony->SetEmpireId((int)m_colonies.size());
	m_colonies[colony->GetEmpireId()] = colony;
}