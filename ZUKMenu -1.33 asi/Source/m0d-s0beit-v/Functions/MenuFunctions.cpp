#include "../stdafx.h"

char* strchr(std::string string)
{
	return ncchr(string.c_str());
}

char* ncchr(const char* constchar)
{
	return _strdup(constchar);
}

Hash $(char* hash)
{
	return GAMEPLAY::GET_HASH_KEY(hash);
}

Hash $(std::string hash)
{
	return GAMEPLAY::GET_HASH_KEY(strchr(hash));
}

float DegToRad(float degs)
{
	return float(degs * M_PI / 180.0f);
}

static std::queue<EntitySpawn> WaitingToSpawn;

void AddEntitySpawn(Hash hash, std::function<void()> spawnFunction)
{
	WaitingToSpawn.push({ hash, spawnFunction });
	STREAMING::REQUEST_MODEL(hash);
}

void UpdateEntitySpawns()
{
	if (!WaitingToSpawn.empty())
	{
		EntitySpawn currSpawn = WaitingToSpawn.front();

		if (STREAMING::HAS_MODEL_LOADED(currSpawn.modelHash))
		{
			currSpawn.spawnFunction();
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(currSpawn.modelHash);
			WaitingToSpawn.pop();
		}
	}
}

void GetInput(int length, std::function<void(char*)> func)
{

	WAIT(0);

	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", length == -1 ? -1 : length + 1);

	/*
		0 - User still editing
		1 - User has finished editing
		2 - User has canceled editing
		3 - Keyboard isn't active
	*/

	while (true)
	{
		int state = GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD();
		if (state == 1)
		{
			char* result = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
			if (strlen(result) > 0)
			{
				func(_strdup(result));
			}
			return;
		}
		if (state != 0)
		{
			return;
		}

		WAIT(0);
	}
}

/*
	0x0000 - Not pressed, not toggled
	0x8000 - Pressed, not toggled
	0x0001 - Not pressed, toggled
	0x8001 - Pressed, toggled
*/

static std::map<DWORD, Key> Keys = // Timings for key repeats
{
	{ INPUT_FRONTEND_LEFT,{ 350, 75 } },
	{ INPUT_FRONTEND_RIGHT,{ 350, 75 } },
	{ INPUT_FRONTEND_UP,{ 350, 100 } },
	{ INPUT_FRONTEND_DOWN,{ 350, 100 } },
	{ INPUT_FRONTEND_ACCEPT,{ 350, 300 } },
	{ INPUT_FRONTEND_CANCEL,{ 999999, 999999 } },
	{ INPUT_FRONTEND_SOCIAL_CLUB,{ 999999, 999999 } },
	{ INPUT_VEH_ACCELERATE,{ 350, 10 } },
	{ INPUT_VEH_BRAKE,{ 350, 10 } },
	{ INPUT_VEH_HANDBRAKE,{ 350, 10 } },
	{ INPUT_VEH_FLY_ROLL_LEFT_ONLY,{ 350, 10 } },
	{ INPUT_VEH_FLY_ROLL_RIGHT_ONLY,{ 350, 10 } },
	{ INPUT_VEH_FLY_PITCH_UP_ONLY,{ 350, 10 } },
	{ INPUT_VEH_FLY_PITCH_DOWN_ONLY,{ 350, 10 } },
	
	//INPUT_VEH_FLY_YAW_LEFT = 89,
	//INPUT_VEH_FLY_YAW_RIGHT
};

static clock_t lastUpdate;

void UpdateKeyPresses()
{
	clock_t currTime = clock();

	double elapsed = max(1, double(currTime - lastUpdate));	// Make sure it's never 0, else you would get key pressed twice
	lastUpdate = currTime;
	for each (auto keyPair in Keys)
	{
		keyPair.second.isPressed = false;
		bool contition = FALSE;
		contition = (keyPair.first == INPUT_FRONTEND_SOCIAL_CLUB) ?
			CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_DOWN) && 
			( CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_CANCEL))
			: CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, keyPair.first);
		if (contition)
		{
			if (keyPair.second.totalDown == 0)
			{
				keyPair.second.isPressed = true;
			}
			else if (keyPair.second.totalDown > keyPair.second.initialDelay)
			{
				if (keyPair.second.sinceLastKeyPress == 0)
				{
					keyPair.second.isPressed = true;
				}
				else if (keyPair.second.sinceLastKeyPress > keyPair.second.repeatSpeed)
				{
					keyPair.second.sinceLastKeyPress -= keyPair.second.repeatSpeed;
					keyPair.second.isPressed = true;
				}
				keyPair.second.sinceLastKeyPress += elapsed;
			}

			keyPair.second.totalDown += elapsed;
		}
		else
		{
			keyPair.second.totalDown = 0;
			keyPair.second.sinceLastKeyPress = 0;
			keyPair.second.isPressed = false;
		}

		Keys[keyPair.first] = keyPair.second; // The for each only gives a 'temp' 
	}
}

bool IsKeyPressed(BYTE key)
{
	return Keys[key].isPressed;
}

bool IsControllerConnected()
{
	return !CONTROLS::_GET_LAST_INPUT_METHOD(2);
}

bool AreStringsEqual(char* string1, char* string2)
{
	return strcmp(string1, string2) == 0;
	
}

bool MenuPairSort(std::pair<std::string, Menu > menu1, std::pair<std::string, Menu > menu2)
{
	return menu1.second.title < menu2.second.title;
}

void hideCellphone()
{
	bool controllerinput_bool = 1;
	MOBILE::DESTROY_MOBILE_PHONE();
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(1);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_NEXT_CAMERA, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_SELECT_NEXT_WEAPON, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_CIN_CAM, controllerinput_bool);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_X);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_ACCEPT);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_CANCEL);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_HUD_SPECIAL, controllerinput_bool);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_DOWN);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_UP);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_ACCEPT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_CANCEL, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_LEFT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_RIGHT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_DOWN, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_UP, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_RDOWN, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_ACCEPT, controllerinput_bool);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(10);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(6);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(7);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(9);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(8);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_LEFT);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_RIGHT);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_PHONE, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_EXTRA_OPTION, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_OPTION, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_UP, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_DOWN, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_LEFT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_RIGHT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_SELECT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CELLPHONE_CANCEL, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_UNARMED, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_MELEE, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_HANDGUN, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SHOTGUN, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SMG, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_AUTO_RIFLE, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SNIPER, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_HEAVY, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SPECIAL, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_WEAPON_WHEEL_NEXT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_WEAPON_WHEEL_PREV, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_WEAPON_SPECIAL_TWO, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_DIVE, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_MELEE_ATTACK_LIGHT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_MELEE_ATTACK_HEAVY, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_MELEE_BLOCK, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_ARREST, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_HEADLIGHT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_RADIO_WHEEL, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CONTEXT, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_RELOAD, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_DIVE, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_CIN_CAM, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_JUMP, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_SELECT_NEXT_WEAPON, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_SELECT_NEXT_WEAPON, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_FRANKLIN, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_MICHAEL, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_TREVOR, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_MULTIPLAYER, controllerinput_bool);
	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CHARACTER_WHEEL, controllerinput_bool);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void GetModelDimensions(Hash modelHash, float& width, float& length, float& height)
{
	Vector3 min, max;
	GAMEPLAY::GET_MODEL_DIMENSIONS(modelHash, &min, &max);
	width = max.y - min.y;
	length = max.x - min.x;
	height = max.z - min.z;
}

Vector3 rot_to_direction(Vector3* rot)
{
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = std::abs((float)std::cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)std::sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)std::cos((double)radiansZ)) * (double)num);
	dir.z = (float)std::sin((double)radiansX);
	return dir;
}

Vector3 add(Vector3* vectorA, Vector3* vectorB)
{
	Vector3 result;
	result.x = vectorA->x;
	result.y = vectorA->y;
	result.z = vectorA->z;

	result.x += vectorB->x;
	result.y += vectorB->y;
	result.z += vectorB->z;

	return result;
}
Vector3 multiply(Vector3* vector, float x)
{
	Vector3 result;
	result.x = vector->x;
	result.y = vector->y;
	result.z = vector->z;

	result.x *= x;
	result.y *= x;
	result.z *= x;

	return result;
}
float get_distance(Vector3* pointA, Vector3* pointB)
{
	float a_x = pointA->x;
	float a_y = pointA->y;
	float a_z = pointA->z;

	float b_x = pointB->x;
	float b_y = pointB->y;
	float b_z = pointB->z;

	double x_ba = (double)(b_x - a_x);
	double y_ba = (double)(b_y - a_y);
	double z_ba = (double)(b_z - a_z);

	double y_2 = y_ba * y_ba;
	double x_2 = x_ba * x_ba;
	double sum_2 = y_2 + x_2;

	return (float)std::sqrt(sum_2 + z_ba);
}

float get_vector_length(Vector3* vector)
{
	double x = (double)vector->x;
	double y = (double)vector->y;
	double z = (double)vector->z;

	return (float)std::sqrt(x * x + y * y + z * z);
}