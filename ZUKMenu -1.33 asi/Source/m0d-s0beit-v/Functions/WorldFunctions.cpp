#include "../stdafx.h"

float Height(Vector3 Coordinate)
{
	float waterheight, groundheight;

	WATER::GET_WATER_HEIGHT(Coordinate.x, Coordinate.y, Coordinate.z, &waterheight);
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(Coordinate.x, Coordinate.y, Coordinate.z, &groundheight ,NULL);

	return waterheight > groundheight ? waterheight : groundheight;
}

void SetWeather(char* weatherType)
{
	GAMEPLAY::SET_WEATHER_TYPE_NOW(weatherType);
}

Menu WeatherMenu()
{
	Menu savedMenu =
	{ "Set weather", "SetWeather",
		{

		}
	};

	for each (char * weatherType in WeatherHashes)
	{
		savedMenu.rows.push_back(enterrow(fmt::format("{0}", weatherType), [=] { SetWeather(weatherType); }));
	}

	return savedMenu;
}

void ToggleWalkRadio(bool state) {
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(state);
}

bool CheckRayCast(float x1, float y1, float z1, float x2,
	float y2, float z2)
{
	/*
	1: Intersect with map
	2: Intersect with mission entities? (includes train)
	4: Intersect with peds? (same as 8)
	8: Intersect with peds? (same as 4)
	10: vehicles
	16: Intersect with objects
	19: Unkown
	32: Unknown
	64: Unknown
	128: Unknown
	256: Intersect with vegetation (plants, coral. trees not included)
	512: Unknown
	*/
	int ray = WORLDPROBE::_CAST_RAY_POINT_TO_POINT(x1, y1, z1, x2, y2, z2, 1+10+256, NULL, 0);
	BOOL *hit = new BOOL();
	Vector3 *endCoords = new Vector3();;
	Vector3 *surfaceNormal = new Vector3();
	Entity *entityHit = new Entity();
	WORLDPROBE::_GET_RAYCAST_RESULT(ray, hit, endCoords,
		surfaceNormal, entityHit);
	return !(*hit);
}

void applyForceToEntity(Entity entity, float x, float y, float z)
{
	ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, x, y, z, 0.0f, 0.0f, 0.0f, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
}