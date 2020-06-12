#pragma once

#include "Object.h"

#include <atomic>

class Planet;

class Colony : public Object
{
public:
	inline Colony() : m_empire_id(0), m_planet(nullptr) {};
	Colony(Planet* planet);
	~Colony();

	bool Run(std::mutex& mutex, std::queue<Object*>& queue);
	void SetEmpireId(int id);
	int GetEmpireId();

private:
	int m_empire_id;
	static std::atomic<int> m_global_id;
	Planet* m_planet;
};

