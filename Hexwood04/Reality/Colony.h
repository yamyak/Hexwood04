#pragma once

#include "Object.h"

#include <atomic>

class Planet;

class Colony : public Object
{
public:
	inline Colony() {};
	Colony(Planet* planet);
	~Colony();

	bool Run();
	void SetEmpireId(int id);
	int GetEmpireId();

private:
	int m_empire_id;
	static std::atomic<int> m_global_id;
	Planet* m_planet;
};

