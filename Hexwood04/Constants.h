#pragma once

#include <string>

namespace Constants
{
	const std::string INITIALIZATION = "INITIALIZATION";
	const std::string SEED = "seed";
	const std::string STAR_DATABASE = "star_db";
	const std::string MANUAL_THREADS = "manual_threads";
	const std::string THREAD_COUNT = "thread_count";

	const std::string SOLAR_SYSTEM = "SOLAR_SYSTEM";
	const std::string MAX_PLANETS = "max_planets";
	const std::string GAS_GIANT_MIN_SIZE = "gas_giant_min_size";
	const std::string GAS_GIANT_MAX_SIZE = "gas_giant_max_size";
	const std::string TERRESTRIAL_MIN_SIZE = "terrestrial_min_size";
	const std::string TERRESTRIAL_MAX_SIZE = "terrestrial_max_size";
	const std::string RESOURCE_SIGMA = "resource_sigma";
	const std::string PLANET_RESOURCES = "planet_resource_values";
	const std::string RESOURCE_MODIFIERS = "planet_resource_modifiers";

	enum class PlanetType
	{
		TERRESTRIAL = 0,
		GAS_GIANT,
		TYPE_MAX
	};

	enum class PlanetEnvironment
	{
		NONE = 0,
		VARIABLE,
		DESERT,
		VOLCANIC,
		OCEAN,
		ICE,
		BARREN,
		ENVIRONMENT_MAX
	};

	enum class ResourceType
	{
		NATURAL_GAS = 0,
		BIOMASS,
		FOSSIL_FUELS,
		HEAVY_METALS,
		WATER,
		RADIOACTIVE,
		RESOURCE_MAX
	};
};