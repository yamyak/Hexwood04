#pragma once

#include <string>

namespace Constants
{
	// km/s
	const float LIGHT_SPEED = 299792.458f;
	const float SECONDS_PER_YEAR = 31556952;

	// ini file keys
	const std::string INITIALIZATION = "INITIALIZATION";
	const std::string SEED = "seed";
	const std::string STAR_DATABASE = "star_db";
	const std::string MANUAL_THREADS = "manual_threads";
	const std::string THREAD_COUNT = "thread_count";
	const std::string LOGGING_FILE = "logging_file";

	const std::string SOLAR_SYSTEM = "SOLAR_SYSTEM";
	const std::string MAX_PLANETS = "max_planets";
	const std::string GAS_GIANT_MIN_SIZE = "gas_giant_min_size";
	const std::string GAS_GIANT_MAX_SIZE = "gas_giant_max_size";
	const std::string TERRESTRIAL_MIN_SIZE = "terrestrial_min_size";
	const std::string TERRESTRIAL_MAX_SIZE = "terrestrial_max_size";
	const std::string RESOURCE_SIGMA = "resource_sigma";
	const std::string PLANET_RESOURCES = "planet_resource_values";
	const std::string RESOURCE_MODIFIERS = "planet_resource_modifiers";
	const std::string RESOURCE_RECHARGE_RATE = "planet_resource_recharge_rate";
	const std::string RECHARGE_RATE_SIGMA = "recharge_rate_sigma";

	const std::string EMPIRE = "EMPIRE";
	const std::string AVERAGE_EMPIRES = "average_empires";
	const std::string EMPIRE_SIGMA = "empire_sigma";

	const std::string COLONY = "COLONY";
	const std::string RESOURCE_CONSUMPTION = "colony_resource_consumption";
	const std::string COLONY_PERIOD_LENGTH = "colony_period_length";
	const std::string COLONY_PERIOD_LENGTH_SIGMA = "colony_period_length_sigma";

	enum class ObjectType
	{
		STAR = 0,
		PLANET,
		EMPIRE,
		COLONY,
		SHIP
	};

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

	enum class CivilizationPeriod
	{
		PREHISTORY,
		STONE_AGE,
		METAL_AGE,
		MACHINE_AGE,
		ATOMIC_AGE,
		SPACE_AGE,
		PERIOD_MAX
	};
};