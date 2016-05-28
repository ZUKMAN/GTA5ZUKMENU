#pragma once

char* strchr(std::string string);
char* ncchr(const char* constchar);

Hash $(char* hash);
Hash $(std::string hash);

Vector3 rot_to_direction(Vector3* rot);
float get_vector_length(Vector3* vector);
float get_distance(Vector3* pointA, Vector3* pointB);
Vector3 multiply(Vector3* vector, float x);
Vector3 add(Vector3* vectorA, Vector3* vectorB);

float DegToRad(float degs);

typedef struct
{
	Hash modelHash;
	std::function<void()> spawnFunction;
} EntitySpawn;

void AddEntitySpawn(Hash hash, std::function<void()> spawnFunction);
void UpdateEntitySpawns();

typedef struct
{
	int initialDelay;
	int repeatSpeed;
	double totalDown;
	double sinceLastKeyPress;
	bool isPressed;
} Key;

void GetInput(int length, std::function<void(char*)> func);
void UpdateKeyPresses();
bool IsKeyPressed(BYTE key);
bool AreStringsEqual(char* string1, char* string2);
bool MenuPairSort(std::pair<std::string, Menu > menu1, std::pair<std::string, Menu > menu2);
void hideCellphone();
bool IsControllerConnected();

template <typename T> T modulo(T m, T n)
{
	return m >= 0 ? m % n : (n - abs(m % n)) % n;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);