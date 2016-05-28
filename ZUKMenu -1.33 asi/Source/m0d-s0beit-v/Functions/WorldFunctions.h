#pragma once

float Height(Vector3 Coordinate);
void SetWeather(char* weatherType);
float DegToRad(float degs);
Menu WeatherMenu();
void ToggleWalkRadio(bool state);
bool CheckRayCast(float x1, float y1, float z1, float x2,
	float y2, float z2);
void applyForceToEntity(Entity entity, float x, float y, float z);
