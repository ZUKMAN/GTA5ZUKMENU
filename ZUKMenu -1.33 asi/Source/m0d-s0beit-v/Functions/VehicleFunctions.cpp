#include "../stdafx.h"

void ToggleRadio(bool state) {
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		AUDIO::SET_VEHICLE_RADIO_ENABLED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), state);
}

void BypassOnlineVehicleKick(Vehicle vehicle,Player player)
{
	int var;
	DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
	DECORATOR::DECOR_REGISTER("Veh_Modded_By_Player", 3);
	DECORATOR::DECOR_SET_INT(vehicle, "Player_Vehicle", NETWORK::_0xBC1D768F2F5D6C05(player));
	DECORATOR::DECOR_SET_INT(vehicle, "Veh_Modded_By_Player", GAMEPLAY::GET_HASH_KEY(PLAYER::GET_PLAYER_NAME(player)));
	DECORATOR::DECOR_SET_INT(vehicle, "Not_Allow_As_Saved_Veh", 0);
	if (DECORATOR::DECOR_EXIST_ON(vehicle, "MPBitset"))
	{
		var = DECORATOR::DECOR_GET_INT(vehicle, "MPBitset");
	}
	GAMEPLAY::SET_BIT(&var, 3);
	DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", var);
	VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, false);
}

void SaveVehiclesToDisk(std::vector<SavedVehicle> savedVehicles)
{
	std::ofstream myfile;
	myfile.open("cars_zuk.bin", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!myfile.is_open()) {
		DrawNotification("Could not open file");
		return;
	}
	size_t size = savedVehicles.size();
	myfile.write((char*)&size, sizeof(size_t));
	for each(SavedVehicle veh in savedVehicles) {
		size = strlen(veh.name);
		myfile.write((char*)&size, sizeof(size_t));
		myfile.write(veh.name, size);
		myfile.write((char*)&veh.hash, sizeof(Hash));
		myfile.write((char*)&veh.wheelType, sizeof(int));

		size = veh.mods.size();
		myfile.write((char*)&size, sizeof(size_t));
		for each (std::pair<int, int> p in veh.mods) {
			myfile.write((char*)&p.first, sizeof(int));
			myfile.write((char*)&p.second, sizeof(int));
		}
		size = veh.toggles.size();
		myfile.write((char*)&size, sizeof(size_t));
		for each (int p in veh.toggles) {
			myfile.write((char*)&p, sizeof(int));
		}
		size = veh.extras.size();
		myfile.write((char*)&size, sizeof(size_t));
		for each (std::pair<int, BOOL> p in veh.extras) {
			myfile.write((char*)&p.first, sizeof(int));
			myfile.write((char*)&p.second, sizeof(BOOL));
		}
		size = strlen(veh.plateText);
		myfile.write((char*)&size, sizeof(size_t));
		myfile.write(veh.plateText, size);

		myfile.write((char*)&veh.plateType, sizeof(int));
		myfile.write((char*)&veh.windowType, sizeof(int));

		myfile.write((char*)&veh.tireSmoke, sizeof(Color));


		size = veh.neons.size();
		myfile.write((char*)&size, sizeof(size_t));
		for each (std::pair<int, BOOL> p in veh.neons) {
			myfile.write((char*)&p.first, sizeof(int));
			myfile.write((char*)&p.second, sizeof(BOOL));
		}

		myfile.write((char*)&veh.neonColor, sizeof(Color));
		myfile.write((char*)&veh.primaryColor, sizeof(int));
		myfile.write((char*)&veh.secondaryColor, sizeof(int));
		myfile.write((char*)&veh.pearlColor, sizeof(int));
		myfile.write((char*)&veh.wheelColor, sizeof(int));
		myfile.write((char*)&veh.customPrimary, sizeof(BOOL));
		myfile.write((char*)&veh.customSecondary, sizeof(BOOL));

		myfile.write((char*)&veh.primaryRGB, sizeof(Color));
		myfile.write((char*)&veh.secondaryRGB, sizeof(Color));

		//DrawNotification(fmt::format("Vehicle {0} saved", veh.name));
	}
	myfile.close();
	DrawNotification(fmt::format("Saved {0} vehicles", savedVehicles.size()));
}

std::vector<SavedVehicle> LoadVehiclesFromDisk()
{
	std::vector<SavedVehicle> savedVehicles;
	std::ifstream myfile;
	myfile.open("cars_zuk.bin", std::ios::in | std::ios::binary);
	if (!myfile.is_open()) {
		DrawNotification("Could not open file");
		return savedVehicles;
	}
	bool read_error = false;
	size_t size = 0;
	myfile.read((char*)&size, sizeof(size_t));
	for (size_t i = 0; i < size;i++) {
		SavedVehicle veh;
		size_t c_size = 0;
		myfile.read((char*)&c_size, sizeof(size_t));
		veh.name = new char[c_size+1];
		myfile.read(veh.name, c_size);
		veh.name[c_size] = '\0';

		myfile.read((char*)&veh.hash, sizeof(Hash));
		myfile.read((char*)&veh.wheelType, sizeof(int));

		myfile.read((char*)&c_size, sizeof(size_t));
		for (size_t c = 0; c < c_size;c++) {
			std::pair<int, int> p;
			myfile.read((char*)&p.first, sizeof(int));
			myfile.read((char*)&p.second, sizeof(int));
			veh.mods.push_back(p);
		}
		myfile.read((char*)&c_size, sizeof(size_t));
		for (size_t c = 0; c < c_size;c++) {
			int p = 0;
			myfile.read((char*)&p, sizeof(int));
			veh.toggles.push_back(p);
		}
		myfile.read((char*)&c_size, sizeof(size_t));
		for (size_t c = 0; c < c_size;c++) {
			std::pair<int, BOOL> p;
			myfile.read((char*)&p.first, sizeof(int));
			myfile.read((char*)&p.second, sizeof(BOOL));
			veh.extras.push_back(p);
		}
		myfile.read((char*)&c_size, sizeof(size_t));
		veh.plateText = new char[c_size+1];
		myfile.read(veh.plateText, c_size);
		veh.plateText[c_size] = '\0';

		myfile.read((char*)&veh.plateType, sizeof(int));
		myfile.read((char*)&veh.windowType, sizeof(int));
		myfile.read((char*)&veh.tireSmoke, sizeof(Color));

		myfile.read((char*)&c_size, sizeof(size_t));
		for (int c = 0; c < c_size;c++) {
			std::pair<int, BOOL> p;
			myfile.read((char*)&p.first, sizeof(int));
			myfile.read((char*)&p.second, sizeof(BOOL));
			veh.neons.push_back(p);
		}
		myfile.read((char*)&veh.neonColor, sizeof(Color));
		myfile.read((char*)&veh.primaryColor, sizeof(int));
		myfile.read((char*)&veh.secondaryColor, sizeof(int));
		myfile.read((char*)&veh.pearlColor, sizeof(int));
		myfile.read((char*)&veh.wheelColor, sizeof(int));
		myfile.read((char*)&veh.customPrimary, sizeof(BOOL));
		myfile.read((char*)&veh.customSecondary, sizeof(BOOL));

		myfile.read((char*)&veh.primaryRGB, sizeof(Color));
		myfile.read((char*)&veh.secondaryRGB, sizeof(Color));

		savedVehicles.push_back(veh);
		//DrawNotification(fmt::format("Vehicle {0} loaded", veh.name));
	}
	myfile.close();
	DrawNotification(fmt::format("Loaded {0} vehicles", savedVehicles.size()));
	return savedVehicles;
}
void BoostBaseVehicleStats(Vehicle vehicle, bool notification)
{
	if (vehicle != NULL)
	{
		RequestControl(vehicle);

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
		VEHICLE::_SET_VEHICLE_PAINT_FADE(vehicle, 0.0f);
		VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE); //What seems to be the officer, problem? *le9gagmemeface*
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, FALSE); //Bulletproof Tires.

		Hash vehicleModel = ENTITY::GET_ENTITY_MODEL(vehicle);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicleModel) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicleModel))
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0); //IDK what this does, but I guess it allows individual mods to be added? It's what the game does before calling SET_VEHICLE_MOD.
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_ENGINE, MOD_INDEX_FOUR, FALSE); //6fast9furious
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_BRAKES, MOD_INDEX_THREE, FALSE); //GOTTA STOP FAST
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_TRANSMISSION, MOD_INDEX_THREE, FALSE); //Not when I shift in to MAXIMUM OVERMEME!
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_SUSPENSION, MOD_INDEX_FOUR, FALSE); //How low can you go?
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_ARMOR, MOD_INDEX_FIVE, FALSE); //100% armor.
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TURBO, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_ARMOR, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_XENONLIGHTS, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TIRESMOKE, TRUE);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, TIRESMOKE_COLOR_PATRIOT);
			VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, WINDOWTINT_BLACK);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, "L33T H4X");
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 0, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 1, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 2, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 3, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, NEON_COLOR_BACKLIGHT);
		}

		RepairVehicle(vehicle);
		SetVehicleModded(vehicle);

		if (notification)
		{
			DrawNotification("Upgraded Vehicle");
		}
	}
}

void RepairVehicle(Vehicle vehicle)
{
	VEHICLE::SET_VEHICLE_FIXED(vehicle);
	VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(vehicle);
	VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(vehicle, 1000.0f);
	VEHICLE::SET_VEHICLE_BODY_HEALTH(vehicle, 1000.0f);
	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, 1000.0f);
	VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
	VEHICLE::_SET_VEHICLE_PAINT_FADE(vehicle, 0.0f);
}

void DumpPlayerFromVehicle(Ped ped, bool notification)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
	{
		//Remove PED from vehicle
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);

		//need to remove the parachute: 0xFBAB5776
		WEAPON::REMOVE_WEAPON_FROM_PED(ped, 0xFBAB5776);
		if (notification)
		{
			std::string selectedPedName = PLAYER::GET_PLAYER_NAME(ped);
			DrawNotification(selectedPedName + "Removed from vehicle");
		}
	}
}

void SetVehicleModded(Vehicle vehicle)
{
	DECORATOR::DECOR_REGISTER("Veh_Modded_By_Player", 3);
	DECORATOR::DECOR_SET_INT(vehicle, "Veh_Modded_By_Player", $(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())));
}

void SpawnPlayerVehicle(Ped playerPed, Hash vehicleHash, bool spawnInVehicle, bool baseUpgrade, bool maxUpgrade, bool preventKick)
{
	AddEntitySpawn(vehicleHash, [=]
	               {
		               RequestControl(playerPed);

		               float currentSpeed = ENTITY::GET_ENTITY_SPEED(playerPed);
		               Vector3 currentVelocity = ENTITY::GET_ENTITY_VELOCITY(playerPed);

		               Vector3 spawnPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 6, 0);

		               if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && spawnInVehicle)
		               {
			               DumpPlayerFromVehicle(playerPed, false);
		               }

		               float groundZ = Height(spawnPos);

		               BOOL isPlane = VEHICLE::IS_THIS_MODEL_A_PLANE(vehicleHash);
		               BOOL isHeli = VEHICLE::IS_THIS_MODEL_A_HELI(vehicleHash);

		               if ((isHeli || isPlane) && spawnInVehicle)
		               {
			               spawnPos.z = max(groundZ + 500, spawnPos.z);
		               }

					   STREAMING::REQUEST_MODEL(vehicleHash);
					   STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
					   while (!STREAMING::HAS_MODEL_LOADED(vehicleHash) && !STREAMING::HAS_PTFX_ASSET_LOADED()) WAIT(0);
					  // if (NETWORK::NETWORK_IS_IN_SESSION())
						//BypassOnlineModelRequestBlock(true);
		               Vehicle vehicle = VEHICLE::CREATE_VEHICLE(vehicleHash, spawnPos.x, spawnPos.y, spawnPos.z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					   // if (NETWORK::NETWORK_IS_IN_SESSION())
						//BypassOnlineModelRequestBlock(false);
		               RequestControl(vehicle);

		               DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
		               DECORATOR::DECOR_REMOVE(vehicle, "Player_Vehicle");

		               VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

		               if (!(isHeli || isPlane))
		               {
			               VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle);
		               }

		               VEHICLE::SET_VEHICLE_ENGINE_ON(vehicle, true, true,true);

		               if (baseUpgrade || maxUpgrade)
		               {
			               BoostBaseVehicleStats(vehicle, false);
			               VEHICLE::SET_VEHICLE_COLOURS(vehicle, COLOR_CLASSIC_SLATE_BLUE, COLOR_CLASSIC_SLATE_BLUE);
			               VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, COLOR_CLASSIC_CANDY_RED, COLOR_CLASSIC_SLATE_BLUE);
		               }
		               if (maxUpgrade)
		               {
			               MaxUpgrade(vehicle);
		               }

		               if (isHeli)
		               {
			               VEHICLE::SET_HELI_BLADES_FULL_SPEED(vehicle);
		               }

					   if (preventKick)
						   BypassOnlineVehicleKick(vehicle, PLAYER::PLAYER_ID());
		               if (spawnInVehicle)
		               {
			               ENTITY::SET_ENTITY_VELOCITY(vehicle, currentVelocity.x, currentVelocity.y, max(-5, currentVelocity.z));
			               PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, SEAT_DRIVER);
			               VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, currentSpeed);

			               if (isHeli || isPlane)
			               {
				               VEHICLE::_SET_VEHICLE_LANDING_GEAR(vehicle, 3);
			               }
		               }


		               ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle);

		               DrawNotification(fmt::format("Spawned {0}", UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicleHash))));
	               });
}

void SpawnSavedVehicle(Ped playerPed, SavedVehicle savedVehicle, bool spawnInVehicle, bool preventKick)
{
	AddEntitySpawn(savedVehicle.hash, [=]
	               {
		               RequestControl(playerPed);

		               float currentSpeed = ENTITY::GET_ENTITY_SPEED(playerPed);
		               Vector3 currentVelocity = ENTITY::GET_ENTITY_VELOCITY(playerPed);

		               Vector3 spawnPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 6, 0);

		               if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && spawnInVehicle)
		               {
			               DumpPlayerFromVehicle(playerPed, false);
		               }

		               float groundZ = Height(spawnPos);

		               BOOL isPlane = VEHICLE::IS_THIS_MODEL_A_PLANE(savedVehicle.hash);
		               BOOL isHeli = VEHICLE::IS_THIS_MODEL_A_HELI(savedVehicle.hash);

		               if ((isHeli || isPlane) && spawnInVehicle)
		               {
			               spawnPos.z = max(groundZ + 500, spawnPos.z);
		               }

					   STREAMING::REQUEST_MODEL(savedVehicle.hash);
					   STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
					   while (!STREAMING::HAS_MODEL_LOADED(savedVehicle.hash) && !STREAMING::HAS_PTFX_ASSET_LOADED()) WAIT(0);
					   //if (NETWORK::NETWORK_IS_IN_SESSION())
						//BypassOnlineModelRequestBlock(true);
		               Vehicle vehicle = VEHICLE::CREATE_VEHICLE(savedVehicle.hash, spawnPos.x, spawnPos.y, spawnPos.z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					   // if (NETWORK::NETWORK_IS_IN_SESSION())
						//BypassOnlineModelRequestBlock(false);
		               RequestControl(vehicle);

		               DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
		               DECORATOR::DECOR_REMOVE(vehicle, "Player_Vehicle");

		               VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

		               if (!(isHeli || isPlane))
		               {
			               VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle);
		               }

		               VEHICLE::SET_VEHICLE_ENGINE_ON(vehicle, true, true,true);

		               if (isHeli)
		               {
			               VEHICLE::SET_HELI_BLADES_FULL_SPEED(vehicle);
		               }

					   if (preventKick)
						   BypassOnlineVehicleKick(vehicle, PLAYER::PLAYER_ID());
		               if (spawnInVehicle)
		               {
			               ENTITY::SET_ENTITY_VELOCITY(vehicle, currentVelocity.x, currentVelocity.y, max(-5, currentVelocity.z));
			               PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, SEAT_DRIVER);
			               VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, currentSpeed);

			               if (isHeli || isPlane)
			               {
				               VEHICLE::_SET_VEHICLE_LANDING_GEAR(vehicle, 3);
			               }
		               }

		               VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, savedVehicle.wheelType);

		               for each (std::pair<int, int> mod in savedVehicle.mods)
		               {
			               SetVehicleMod(vehicle, mod.first, mod.second);
		               }

					   for each (int toggle in savedVehicle.toggles)
		               {
						   VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, toggle, TRUE);
		               }

		               for each (std::pair<int, BOOL> extra in savedVehicle.extras)
		               {
			               VEHICLE::SET_VEHICLE_EXTRA(vehicle, extra.first, extra.second == TRUE ? 0 : -1);
		               }

		               VEHICLE::SET_VEHICLE_COLOURS(vehicle, savedVehicle.primaryColor, savedVehicle.secondaryColor);
		               VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, savedVehicle.pearlColor, savedVehicle.wheelColor);
					   //VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(vehicle, savedVehicle.interiorColor);
		               if (savedVehicle.customPrimary)
		               {
			               VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, savedVehicle.primaryRGB.red, savedVehicle.primaryRGB.green, savedVehicle.primaryRGB.blue);
		               }

		               if (savedVehicle.customSecondary)
		               {
			               VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, savedVehicle.secondaryRGB.red, savedVehicle.secondaryRGB.green, savedVehicle.secondaryRGB.blue);
		               }

		               VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, savedVehicle.tireSmoke.red, savedVehicle.tireSmoke.green, savedVehicle.tireSmoke.blue);

		               for each (std::pair<int, BOOL> neon in savedVehicle.neons)
		               {
			               VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, neon.first, neon.second);
		               }

		               VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, savedVehicle.neonColor.red, savedVehicle.neonColor.green, savedVehicle.neonColor.blue);

		               VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, savedVehicle.plateType);

		               VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, savedVehicle.plateText);

		               VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, savedVehicle.windowType);

		               SetVehicleModded(vehicle);

		               ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle);

		               DrawNotification(fmt::format("Spawned {0}", savedVehicle.name));
	               });
}

SavedVehicle SaveVehicle(char* name, Vehicle vehicle)
{
	/*
		char* name;
		Hash hash;
		int wheelType;
		std::vector<std::pair<int, int>> mods;
		std::vector<std::pair<int, bool>> extras;
		char* plateText;
		int plateType;
		int windowType;
		color_t tireSmoke;
		std::vector<std::pair<int, bool>> neons;
		color_t neonColor;
		int primaryColor;
		int secondaryColor;
		int pearlColor;
		int wheelColor;
		bool customPrimary;
		bool customSecondary;
		color_t primaryRGB;
		color_t secondaryRGB;	
	*/

	std::vector<std::pair<int, int>> mods;

	for (int i = 0; i < 50; i++)
	{
		if (VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) > 0)
		{
			mods.push_back({i, VEHICLE::GET_VEHICLE_MOD(vehicle, i)});
		}
	}

	std::vector<int> toggles;

	for (int i = 0; i < 50; i++)
	{
		if (VEHICLE::IS_TOGGLE_MOD_ON(vehicle, i))
		{
			toggles.push_back(i);
		}
	}

	std::vector<std::pair<int, BOOL>> extras;

	for (int i = 0; i < 12; i++)
	{
		if (VEHICLE::DOES_EXTRA_EXIST(vehicle, i))
		{
			extras.push_back({i, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, i)});
		}
	}

	int tireR, tireG, tireB;

	VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, &tireR, &tireG, &tireB);

	std::vector<std::pair<int, BOOL>> neons;

	for (int i = 0; i < 5; i++)
	{
		neons.push_back({ i, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(vehicle, i) });
	}

	int neonR, neonG, neonB;

	VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, &neonR, &neonG, &neonB);

	int primary, secondary;

	VEHICLE::GET_VEHICLE_COLOURS(vehicle, &primary, &secondary);

	int pearl, wheel;

	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &pearl, &wheel);

	int primR, primG, primB;

	VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, &primR, &primG, &primB);

	int secR, secG, secB;

	VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, &secR, &secG, &secB);

	return
	{
		name,
		ENTITY::GET_ENTITY_MODEL(vehicle),
		VEHICLE::GET_VEHICLE_WHEEL_TYPE(vehicle),
		mods,
		toggles,
		extras,
		_strdup(VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(vehicle)),
		VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle),
		VEHICLE::GET_VEHICLE_WINDOW_TINT(vehicle),
		{tireR, tireG, tireB},
		neons,
		{neonR, neonG, neonB},
		primary,
		secondary,
		pearl,
		wheel,
		VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(vehicle),
		VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(vehicle),
		{primR, primG, primB},
		{secR, secG, secB}
	};
}

Menu SavedMenu(std::vector<SavedVehicle> savedVehicles, Ped selfPed, bool * spawnInVehicle,bool preventKick)
{
	Menu savedMenu = 
	{ "Saved Vehicles", spawnInVehicle == NULL ? "Selected Player" : "Vehicle",
		{

		}
	};

	for each (SavedVehicle savedVehicle in savedVehicles)
	{
		if(spawnInVehicle == NULL)
			savedMenu.rows.push_back(enterrow(fmt::format("Spawn {0}", savedVehicle.name), [=] { SpawnSavedVehicle(selfPed, savedVehicle, false, false); }));
		else
			savedMenu.rows.push_back(enterrow(fmt::format("Spawn {0}", savedVehicle.name), [=] { SpawnSavedVehicle(selfPed, savedVehicle, *spawnInVehicle, preventKick); }));
	}

	return savedMenu;
}

void SetPlateType(Vehicle vehicle, PlateTypes plateType)
{
	if (vehicle != NULL)
	{
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, plateType);
		SetVehicleModded(vehicle);
	}
}

void SetWindowTint(Vehicle vehicle, WindowTints tint)
{
	VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, tint);
	SetVehicleModded(vehicle);
}

void ResprayVehicle(Vehicle vehicle, int primary, int secondary, int pearl, int wheel)
{
	VEHICLE::SET_VEHICLE_COLOURS(vehicle, primary, secondary);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, pearl, wheel);
	SetVehicleModded(vehicle);
	DECORATOR::DECOR_SET_BOOL(vehicle, "Sprayed_Veicle_Decorator", TRUE);
	DECORATOR::DECOR_SET_TIME(vehicle, "Sprayed_Vehicle_Timer_Dec", NETWORK::GET_NETWORK_TIME());
}

void SetVehiclePrimary(Vehicle vehicle, int color)
{
	int currPrim, currSec;
	VEHICLE::GET_VEHICLE_COLOURS(vehicle, &currPrim, &currSec);
	VEHICLE::SET_VEHICLE_COLOURS(vehicle, color, currSec);
}

void SetVehicleSecondary(Vehicle vehicle, int color)
{
	int currPrim, currSec;
	VEHICLE::GET_VEHICLE_COLOURS(vehicle, &currPrim, &currSec);
	VEHICLE::SET_VEHICLE_COLOURS(vehicle, currPrim, color);
}

void SetVehiclePearl(Vehicle vehicle, int color)
{
	int currPearl, currWheelCol;
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &currPearl, &currWheelCol);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, color, currWheelCol);
}

void SetVehicleWheelColor(Vehicle vehicle, int color)
{
	int currPearl, currWheelCol;
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &currPearl, &currWheelCol);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, currPearl, color);
}

void ToggleExtra(Vehicle vehicle, int extraID)
{
	if (VEHICLE::DOES_EXTRA_EXIST(vehicle, extraID))
	{
		RequestControl(vehicle);
		VEHICLE::SET_VEHICLE_EXTRA(vehicle, extraID, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, extraID) ? -1 : 0);
		SetVehicleModded(vehicle);
	}
}

void ClaimVehicle(Vehicle vehicle)
{
	RequestControl(vehicle);

	//DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
	//DECORATOR::DECOR_REGISTER("PV_Slot", 3);
	DECORATOR::DECOR_SET_INT(vehicle, "Player_Vehicle", NETWORK::_0xBC1D768F2F5D6C05(PLAYER::PLAYER_ID()));
	DECORATOR::DECOR_SET_INT(vehicle, "PV_Slot", 0);
	DECORATOR::DECOR_SET_INT(vehicle, "Not_Allow_As_Saved_Veh", $(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())));
}

void SetNumberPlate(Vehicle vehicle, char* text)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, text);
	SetVehicleModded(vehicle);
}

bool DoesVehicleExist(char* name)
{
	//return !AreStringsEqual("CARNOTFOUND", VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL($(name)));
	return STREAMING::IS_MODEL_A_VEHICLE($(name)) == TRUE;
}

void SetVehicleMod(Vehicle vehicle, int modType, int modIndex)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_MOD(vehicle, modType, modIndex, TRUE);
}

void ToggleVehicleMod(Vehicle vehicle, int modType)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, !VEHICLE::IS_TOGGLE_MOD_ON(vehicle, modType));
}

void SetVehicleToggle(Vehicle vehicle, int modType, BOOL toggle)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, toggle);
}

void SetVehicleLivery(Vehicle vehicle, int livery)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_LIVERY(vehicle, livery);
}

void SetTireSmoke(Vehicle vehicle, int r, int g, int b)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, r, g, b);
}

void ToggleNeonLight(Vehicle vehicle, int index)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, index, !VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(vehicle, index));
}

void MaxUpgrade(Vehicle vehicle)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

	for (int i = 0; i < 50; i ++)
	{
		SetVehicleMod(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1);
	}
}

void VehicleFly(Vehicle playerVeh, float & yaw)
{
	float rotation, pitch, roll = 0.0f;
	if (playerVeh != NULL)
	{
		VEHICLE::SET_VEHICLE_GRAVITY(playerVeh, FALSE); // No more falling.
		pitch = ENTITY::GET_ENTITY_PITCH(playerVeh);
		float curSpeed = ENTITY::GET_ENTITY_SPEED(playerVeh);
		bool isBackward =false;

		if (IsKeyPressed(INPUT_VEH_ACCELERATE)) /* W DOWN - GO FORWARD */
		{
			isBackward = false;
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch, 0.0f, yaw, 2, TRUE);
			RequestControl(playerVeh);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(playerVeh, curSpeed += 3.0f);
		}
		else if (IsKeyPressed(INPUT_VEH_BRAKE)) /* S DOWN - GO BACKWARD */
		{
			isBackward = true;
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch, 0.0f, yaw, 2, TRUE);
			RequestControl(playerVeh);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(playerVeh, (curSpeed * -1.0f) - 3.0f);
		}
		else
		{
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch, roll, yaw, 2, TRUE);
			if (curSpeed > 3.0f && isBackward)
			{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(playerVeh, curSpeed - 3.0f);
			}
			else
			{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(playerVeh, 0.0f);
			}
		}

		if (IsKeyPressed(INPUT_VEH_FLY_ROLL_LEFT_ONLY)) /* A DOWN - TURN LEFT */
		{
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch, roll, yaw += 4.5f, 2, TRUE);
		}

		if (IsKeyPressed(INPUT_VEH_FLY_ROLL_RIGHT_ONLY)) /* D DOWN - TURN RIGHT */
		{
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch, roll, yaw -= 4.5f, 2, TRUE);
		}

		if (IsKeyPressed(INPUT_VEH_FLY_PITCH_UP_ONLY)) /* SHIFT DOWN - PITCH UP */
		{
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch -= 4.5f, roll, yaw, 2, TRUE);
		}

		if (IsKeyPressed(INPUT_VEH_FLY_PITCH_DOWN_ONLY)) /* CTRL DOWN - PITCH DOWN */
		{
			ENTITY::SET_ENTITY_ROTATION(playerVeh, pitch += 4.5f, roll, yaw, 2, TRUE);
		}

		if (IsKeyPressed(INPUT_VEH_HANDBRAKE)) /* SPACE DOWN - STOP */
		{
			ENTITY::SET_ENTITY_ROTATION(playerVeh, 0.0f, roll, yaw, 2, TRUE);
			RequestControl(playerVeh);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(playerVeh, 0.0f);
		}
	}
	else
	{
		VEHICLE::SET_VEHICLE_GRAVITY(playerVeh, TRUE);
	}
}

std::vector<std::pair<std::string, Menu>> VehicleCustomMenu(Vehicle vehicle)
{
#pragma region PartMaps

	std::map<std::string, int> VehicleModMap =
		{
				{"Spoiler", 0},
				{"Front Bumper", 1},
				{"Rear Bumper", 2},
				{"Skirts", 3},
				{"Exhaust", 4},
				{"Chassis", 5},
				{"Grille", 6},
				{"Hood", 7},
				{"Fender", 8},
				{"Fender Ornaments", 9},
				{"Roof", 10},
				{"Engine", 11},
				{"Brakes", 12},
				{"Transmission", 13},
				{"Horn", 14},
				{"Suspension", 15},
				{"Armour", 16},
				//{"Unknown1", 17},
				//{"Unknown2", 18},
				//{"Unknown3", 19},
				//{"Unknown4", 20},
				//{"Unknown5", 21},
				//{"Unknown6", 22},
				{"Wheels", 23},
				{"Second Wheel", 24},
				{"Plateholder", 25},
				{"Vanity Plates", 26},
				{"Trim Design", 27},
				{"Bobbleheads", 28},
				{"Unknown", 29},
				{"Dials", 30},
				{"Speakers", 31},
				{"Seats", 32},
				{"Steering Wheels", 33},
				{"Column Shifter", 34},
				{"Plaques", 35},
				{"Speakers", 36},
				{"Trunk Speakers", 37},
				{"Hydraulics", 38},
				{"Engine Covers", 39},
				{"Engine Filters", 40},
				{"Unknown7", 41},
				{"Arch Covers", 42},
				{"Aerials", 43},
				{"Trim", 44},
				{"Tanks", 45},
				{"Unknown8", 46},
				{"Unknown9", 47},
				{"Livery", 48},
		};

	std::map<std::string, int> VehicleModToggles =
		{
				{"Turbo", 18},
				{"Xenon Lights", 22},
		};

	std::map<int, std::map<int, char*>> manualStrings =
		{
				{1,
						{
								{0, "Custom Front Bumper"},
						}
				},
				{2,
						{
								{0, "Custom Rear Bumper"},
						}
				},
				{3,
						{
								{0, "Custom Skirts"},
						}
				},
				{11,
						{
								{-1, "Engine Upgrade, Level 0"},
								{0,  "Engine Upgrade, Level 1"},
								{1,  "Engine Upgrade, Level 2"},
								{2,  "Engine Upgrade, Level 3"},
								{3,  "Engine Upgrade, Level 4"},
						}
				},
				{12,
						{
								{-1, "Stock Brakes"},
								{0,  "Street Brakes"},
								{1,  "Sport Brakes"},
								{2,  "Race Brakes"},
						}
				},
				{13,
						{
								{-1, "Stock Transmisison"},
								{0,  "Street Transmission"},
								{1,  "Sports Transmission"},
								{2,  "Race Transmission"},
						}
				},
				{14,
						{
								{-1, "Stock"},
								{0,  "Truck Horn"},
								{1,  "Cop Horn"},
								{2,  "Clown Horn"},
								{8,  "Sad Trombone"},
								{3,  "Musical Horn 1"},
								{4,  "Musical Horn 2"},
								{5,  "Musical Horn 3"},
								{6,  "Musical Horn 4"},
								{7,  "Musical Horn 5"},
								{9,  "Classical Horn 1"},
								{10, "Classical Horn 2"},
								{11, "Classical Horn 3"},
								{12, "Classical Horn 4"},
								{13, "Classical Horn 5"},
								{14, "Classical Horn 6"},
								{15, "Classical Horn 7"},
								{33, "Classical Horn 8"},
								{32, "Classical Horn Loop 1"},
								{34, "Classical Horn Loop 2"},
								{16, "Scale - Do"},
								{17, "Scale - Ra"},
								{18, "Scale - Mi"},
								{19, "Scale - Fa"},
								{20, "Scale - Sol"},
								{21, "Scale - La"},
								{22, "Scale - Ti"},
								{23, "Scale - Do (High)"},
								{24, "Jazz Horn 1"},
								{25, "Jazz Horn 2"},
								{26, "Jazz Horn 3"},
								{27, "Jazz Horn Loop"},
								{28, "Star Spangled Banner 1"},
								{29, "Star Spangled Banner 2"},
								{30, "Star Spangled Banner 3"},
								{31, "Star Spangled Banner 4"},
								{38, "Halloween Loop 1"},
								{40, "Halloween Loop 2"},
								{42, "San Andreas"},
								{44, "Liberty City"},
								{35, "Classical Horn Loop 1 (Non Loop)	"},
								{37, "Classical Horn Loop 2 (Non Loop)"},
								{36, "Classical Horn 8 (Start)"},
								{39, "Halloween Loop 1 (Non Loop)"},
								{41, "Halloween Loop 2 (Non Loop)"},
								{43, "San Andreas (Non Loop)"},
								{45, "Liberty City (Non Loop)"},
								{46, "Xmas 1"},
								{47, "Xmas 2"},
								{ 48, "48Liberty City (Non Loop)" },
								{ 49, "49Liberty City (Non Loop)" },
						}
				},
				{15,
						{
								{-1, "Stock Suspension"},
								{0,  "Lowered Suspension"},
								{1,  "Street Suspension"},
								{2,  "Sport Suspension"},
								{3,  "Competition Suspension"},
								{4,  "Race Suspension"},
						}
				},
				{16,
						{
								{-1, "No Armor"},
								{0,  "20% Armor"},
								{1,  "40% Armor"},
								{2,  "60% Armor"},
								{3,  "80% Armor"},
								{4,  "100% Armor"},
						}
				},
				{18,
					{
						{-1, "None"},
						{0,  "Turbo Tuning" }
					}
				},
				{22,
					{
						{-1, "Stock Lights"},
						{0,  "Xenon Lights" }
					}
				},
		};

#pragma region Paint

	std::map<std::string, int> VehicleColorsClassic =
		{
				{"Black", 0},
				{"Graphite", 1},
				{"Black Steel", 2},
				{"Dark Steel", 3},
				{"Silver", 4},
				{"Bluish Silver", 5},
				{"Rolled Steel", 6},
				{"Shadow Silver", 7},
				{"Stone Silver", 8},
				{"Midnight Silver", 9},
				{"Cast Iron Silver", 10},
				{"Anhracite Black", 11},
				{"Red", 27},
				{"Torino Red", 28},
				{"Formula Red", 29},
				{"Blaze Red", 30},
				{"Grace Red", 31},
				{"Garnet Red", 32},
				{"Sunset Red", 33},
				{"Cabernet Red", 34},
				{"Candy Red", 35},
				{"Sunrise Orange", 36},
				{"Gold", 37},
				{"Orange", 38},
				{"Dark Green", 49},
				{"Racing Green", 50},
				{"Sea Green", 51},
				{"Olive Green", 52},
				{"Bright Green", 53},
				{"Gasoline Green", 54},
				{"Pale Dark Green", 56},
				{"Midnight Green", 59},
				{"Metallic Dark Blue", 60},
				{"Galaxy Blue", 61},
				{"Dark Blue", 62},
				{"Saxon Blue", 63},
				{"Blue", 64},
				{"Mariner Blue", 65},
				{"Harbor Blue", 66},
				{"Diamond Blue", 67},
				{"Surf Blue", 68},
				{"Nautical Blue", 69},
				{"Ultra Blue", 70},
				{"Schafter Purple", 71},
				{"Spinnaker Purple", 72},
				{"Racing Blue", 73},
				{"Light Blue", 74},
				{"Slate Blue", 81},
				{"Baby Blue", 87},
				{"Yellow", 88},
				{"Race Yellow", 89},
				{"Bronze", 90},
				{"Dew Yellow", 91},
				{"Lime Green", 92},
				{"Champagne", 93},
				{"Feltzer Brown", 94},
				{"Creeen Brown", 95},
				{"Chocolate Brown", 96},
				{"Maple Brown", 97},
				{"Saddle Brown", 98},
				{"Straw Brown", 99},
				{"Moss Brown", 100},
				{"Bison Brown", 101},
				{"Woodbeech Brown", 102},
				{"Beechwood Brown", 103},
				{"Sienna Brown", 104},
				{"Sandy Brown", 105},
				{"Bleached Brown", 106},
				{"Cream", 107},
				{"Ice White", 111},
				{"Frost White", 112},
				{"Dark Brown", 115},
				{"Pea Green", 125},
				{"Police Blue", 127},
				{"Pure White", 134},
				{"Hot Pink", 135},
				{"Salmon Pink", 136},
				{"Pfsiter Pink", 137},
				{"Bright Orange", 138},
				{"Green", 139},
				{"Flourescent Blue", 140},
				{"Midnight Blue", 141},
				{"Midnight Purple", 142},
				{"Wine Red", 143},
				{"Hunter Green", 144},
				{"Bright Purple", 145},
				{"Midnight Galaxy Blue", 146},
				{"Carbon Black", 147},
				{"Lava Red", 150},
				{"Epsilon Blue", 157},
		};

	std::map<std::string, int> VehicleColorsWorn =
		{
				{"Black", 21},
				{"Graphite", 22},
				{"Silver Grey", 23},
				{"Silver", 24},
				{"Blue Silver", 25},
				{"Shadow Silver", 26},
				{"Red", 46},
				{"Golden Red", 47},
				{"Dark Red", 48},
				{"Green", 57},
				{"Sea Wash", 58},
				{"Dark Blue", 85},
				{"Blue", 86},
				{"Honey Beige", 113},
				{"Brown", 114},
				{"Straw Beige", 116},
				{"Off White", 121},
				{"Orange", 123},
				{"Light Orange", 124},
				{"Taxi Yellow", 126},
				{"Pale Orange", 130},
				{"White", 132},
				{"Army Olive Green", 133},
		};

	std::map<std::string, int> VehicleColorsUtil =
		{
				{"Black", 15},
				{"Black Poly", 16},
				{"Dark Silver", 17},
				{"Gun Metal", 19},
				{"Shadow Silver", 20},
				{"Red", 43},
				{"Bright Red", 44},
				{"Garnet Red", 45},
				{"Midnight Blue", 75},
				{"Blue", 76},
				{"Sea Foam Blue", 77},
				{"Lightning Blue", 78},
				{"Maui Blue Poly", 79},
				{"Bright Blue", 80},
				{"Brown", 108},
				{"Medium Brown", 109},
				{"Light Brown", 110},
				{"Off White", 122},
		};

	std::map<std::string, int> VehicleColorsMatte =
		{
				{"Black", 12},
				{"Gray", 13},
				{"Light Gray", 14},
				{"Red", 39},
				{"Dark Red", 40},
				{"Orange", 41},
				{"Yellow", 42},
				{"Lime Green", 55},
				{"Dark Blue", 82},
				{"Blue", 83},
				{"Midnight Blue", 84},
				{"Green", 128},
				{"Brown", 129},
				{"Ice White", 131},
				{"Schafter Purple", 148},
				{"Midnight Purple", 149},
				{"Forest Green", 151},
				{"Olive Drab", 152},
				{"Dark Earth", 153},
				{"Desert Tan", 154},
				{"Foliage Green", 155},
		};

	std::map<std::string, int> VehicleColorsMetals =
		{
				{"Pearlescent Steel", 18},
				{"Brushed Steel", 117},
				{"Brushed Black Steel", 118},
				{"Brushed Aluminum", 119},
				{"Default Alloy", 156},
				{"Pure Gold", 158},
				{"Brushed Gold", 159},
				{"Pearlescent Gold", 160},
				{"Chrome", 120},
		};

#pragma endregion

#pragma endregion

	std::vector<std::pair<std::string, Menu>> menus = {};

	Menu menu = {"shopui_title_carmod2@shopui_title_carmod2@SPRITE", "Vehicle", {}};

	if (vehicle != NULL)
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

#pragma region Vehicle Mods

		for each (std::pair<std::string, int> mod in VehicleModMap)
		{
			int modIndex = mod.second;
			int modCount = VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, modIndex);

			char* key = strchr(fmt::format("VehicleMod{0}", mod.first));

			if (modCount > 0)
			{
				Menu modMenu = {strchr(mod.first) , "VehicleCustomisation", {}};

				std::map<int, char*> predefined = manualStrings[modIndex];

				modMenu.rows.push_back(enterrow(predefined[-1] == nullptr ? "Default" : std::string(predefined[-1]), [=]
				                                {
					                                SetVehicleMod(vehicle, modIndex, -1);
				                                }));

				for (int i = 0; i < modCount; i++)
				{
					char* modCodeName = VEHICLE::GET_MOD_TEXT_LABEL(vehicle, modIndex, i);
					char* modDisplayName = UI::_GET_LABEL_TEXT(modCodeName);

					if (AreStringsEqual(modDisplayName, "NULL"))
					{
						if (predefined[i] != nullptr)
						{
							modDisplayName = predefined[i];
						}
						else
						{
							modDisplayName = strchr(fmt::format("{0} {1}", mod.first, i));
						}
					}

					modMenu.rows.push_back(enterrow(std::string(modDisplayName), [=]
					                                {
						                                SetVehicleMod(vehicle, modIndex, i);
					                                }));
				}



				menu.rows.push_back(menurow(fmt::format("{0} ({1})", mod.first, modCount), key));

				menus.push_back({key, modMenu});
			}
		}

#pragma endregion

#pragma region Wheel Type

		Menu wheelMenu =
			{"Wheel Type", "VehicleCustomisation",
					{
						enterrow("Sport", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 0);
						         }),
						enterrow("Muscle", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 1);
						         }),
						enterrow("Lowrider", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 2);
						         }),
						enterrow("SUV", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 3);
						         }),
						enterrow("Offroad", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 4);
						         }),
						enterrow("Tuner", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 5);
						         }),
						enterrow("Bike Wheels", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 6);
						         }),
						enterrow("High End", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 7);
						         }),
						enterrow("Benny's originals", [=]
								 {
									 VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 8);
								 }),
						enterrow("Benny's exclusive", [=]
								 {
									 VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 9);
								 }),
					}
			};

		menus.push_back({"VehicleModWheelType", wheelMenu});

		menu.rows.push_back(menurow("Wheel Type", "VehicleModWheelType"));

#pragma endregion

#pragma region Respray

		Menu resprayMenu =
			{"Respray", "VehicleCustomisation",
					{
						menurow("Primary", "VehiclePrimSel"),
						menurow("Secondary", "VehicleSecSel"),
						menurow("Pearl", "VehiclePearlSel"),
						menurow("Wheel", "VehicleWheelSel"),
					}
			};

		menus.push_back({"VehicleRespray", resprayMenu});

		menu.rows.push_back(menurow("Respray", "VehicleRespray"));

#pragma region Respray Primary

		static int primaryR, primaryG, primaryB;

		Menu primMenu =
			{"Primary Colors", "VehicleRespray",
					{
						menurow("Classic", "VehiclePrimClassic"),
						menurow("Worn", "VehiclePrimWorn"),
						menurow("Utils", "VehiclePrimUtils"),
						menurow("Matte", "VehiclePrimMatte"),
						menurow("Metals", "VehiclePrimMetals"),
						menurow("Predefined custom", "VehiclePrimPredefined"),
						introw("Primary Red", &primaryR, 0, 255, 1),
						introw("Primary Green", &primaryG, 0, 255, 1),
						introw("Primary Blue", &primaryB, 0, 255, 1),
						enterrow("Set Primary Color", [&, vehicle]
						         {
							         VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, primaryR, primaryG, primaryB);
						         }),
						enterrow("Clear Custom Primary", [=]
						         {
							         VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle);
						         }),
					}
			};

		menus.push_back({"VehiclePrimSel", primMenu});

		Menu primMenuClassic = {"Classic", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			primMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                        {
				                                        SetVehiclePrimary(vehicle, color.second);
			                                        }));
		}

		sort(primMenuClassic.rows.begin(), primMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimClassic", primMenuClassic});

		Menu primMenuWorn = {"Worn", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			primMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                     {
				                                     SetVehiclePrimary(vehicle, color.second);
			                                     }));
		}

		sort(primMenuWorn.rows.begin(), primMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimWorn", primMenuWorn});

		Menu primMenuUtils = {"Utils", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			primMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehiclePrimary(vehicle, color.second);
			                                      }));
		}

		sort(primMenuUtils.rows.begin(), primMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimUtils", primMenuUtils});

		Menu primMenuMatte = {"Matte", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			primMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehiclePrimary(vehicle, color.second);
			                                      }));
		}

		sort(primMenuMatte.rows.begin(), primMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimMatte", primMenuMatte});

		Menu primMenuMetals = {"Metals", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			primMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehiclePrimary(vehicle, color.second);
			                                       }));
		}

		sort(primMenuMetals.rows.begin(), primMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimMetals", primMenuMetals});

		Menu primMenuPredefined = { "Predefined custom", "VehiclePrimSel",{} };

		for each (std::pair<std::string, color_t> color in allColors)
		{
			primMenuPredefined.rows.push_back(enterrow(color.first, [=]
			{
				primaryR = color.second.red;
				primaryG = color.second.green;
				primaryB = color.second.blue;
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, primaryR, primaryG, primaryB);
			}));
		}

		sort(primMenuPredefined.rows.begin(), primMenuPredefined.rows.end(), MenuRowSort);

		menus.push_back({ "VehiclePrimPredefined", primMenuPredefined });

#pragma endregion

#pragma region Respray Secondary

		static int secondaryR, secondaryG, secondaryB;

		Menu secMenu =
			{"Secondary Colors", "VehicleRespray",
					{
						menurow("Classic", "VehicleSecClassic"),
						menurow("Worn", "VehicleSecWorn"),
						menurow("Utils", "VehicleSecUtils"),
						menurow("Matte", "VehicleSecMatte"),
						menurow("Metals", "VehicleSecMetals"),
						menurow("Predefined custom", "VehicleSecPredefined"),
						introw("Secondary Red", &secondaryR, 0, 255, 1),
						introw("Secondary Green", &secondaryG, 0, 255, 1),
						introw("Secondary Blue", &secondaryB, 0, 255, 1),
						enterrow("Set Secondary Color", [&, vehicle]
						         {
							         VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, secondaryR, secondaryG, secondaryB);
						         }),
						enterrow("Clear Custom Secondary", [=]
						         {
							         VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle);
						         }),
					}
			};

		menus.push_back({"VehicleSecSel", secMenu});

		Menu secMenuClassic = {"Classic", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			secMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehicleSecondary(vehicle, color.second);
			                                       }));
		}

		sort(secMenuClassic.rows.begin(), secMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecClassic", secMenuClassic});

		Menu secMenuWorn = {"Worn", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			secMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                    {
				                                    SetVehicleSecondary(vehicle, color.second);
			                                    }));
		}

		sort(secMenuWorn.rows.begin(), secMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecWorn", secMenuWorn});

		Menu secMenuUtils = {"Utils", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			secMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                     {
				                                     SetVehicleSecondary(vehicle, color.second);
			                                     }));
		}

		sort(secMenuUtils.rows.begin(), secMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecUtils", secMenuUtils});

		Menu secMenuMatte = {"Matte", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			secMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                     {
				                                     SetVehicleSecondary(vehicle, color.second);
			                                     }));
		}

		sort(secMenuMatte.rows.begin(), secMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecMatte", secMenuMatte});

		Menu secMenuMetals = {"Metals", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			secMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehicleSecondary(vehicle, color.second);
			                                      }));
		}

		sort(secMenuMetals.rows.begin(), secMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecMetals", secMenuMetals});

		Menu secMenuPredefined = { "Predefined custom", "VehicleSecSel",{} };

		for each (std::pair<std::string, color_t> color in allColors)
		{
			secMenuPredefined.rows.push_back(enterrow(color.first, [=]
			{
				secondaryR = color.second.red;
				secondaryG = color.second.green;
				secondaryB = color.second.blue;
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, secondaryR, secondaryG, secondaryB);
			}));
		}

		sort(secMenuPredefined.rows.begin(), secMenuPredefined.rows.end(), MenuRowSort);

		menus.push_back({ "VehicleSecPredefined", secMenuPredefined });

#pragma endregion

#pragma region Respray Pearl

		Menu pearlMenu =
			{"Pearl Colors", "VehicleRespray",
					{
						menurow("Classic", "VehiclePearlClassic"),
						menurow("Worn", "VehiclePearlWorn"),
						menurow("Utils", "VehiclePearlUtils"),
						menurow("Matte", "VehiclePearlMatte"),
						menurow("Metals", "VehiclePearlMetals"),
					}
			};

		menus.push_back({"VehiclePearlSel", pearlMenu});

		Menu pearlMenuClassic = {"Classic", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			pearlMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                         {
				                                         SetVehiclePearl(vehicle, color.second);
			                                         }));
		}

		sort(pearlMenuClassic.rows.begin(), pearlMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlClassic", pearlMenuClassic});

		Menu pearlMenuWorn = {"Worn", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			pearlMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehiclePearl(vehicle, color.second);
			                                      }));
		}

		sort(pearlMenuWorn.rows.begin(), pearlMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlWorn", pearlMenuWorn});

		Menu pearlMenuUtils = {"Utils", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			pearlMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehiclePearl(vehicle, color.second);
			                                       }));
		}

		sort(pearlMenuUtils.rows.begin(), pearlMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlUtils", pearlMenuUtils});

		Menu pearlMenuMatte = {"Matte", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			pearlMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehiclePearl(vehicle, color.second);
			                                       }));
		}

		sort(pearlMenuMatte.rows.begin(), pearlMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlMatte", pearlMenuMatte});

		Menu pearlMenuMetals = {"Metals", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			pearlMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                        {
				                                        SetVehiclePearl(vehicle, color.second);
			                                        }));
		}

		sort(pearlMenuMetals.rows.begin(), pearlMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlMetals", pearlMenuMetals});

#pragma endregion

#pragma region Respray Wheel

		Menu wheelPaintMenu =
			{"Wheel Colors", "VehicleRespray",
					{
						menurow("Classic", "VehicleWheelClassic"),
						menurow("Worn", "VehicleWheelWorn"),
						menurow("Utils", "VehicleWheelUtils"),
						menurow("Matte", "VehicleWheelMatte"),
						menurow("Metals", "VehicleWheelMetals"),
					}
			};

		menus.push_back({"VehicleWheelSel", wheelPaintMenu});

		Menu wheelMenuClassic = {"Classic", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			wheelMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                         {
				                                         SetVehicleWheelColor(vehicle, color.second);
			                                         }));
		}

		sort(wheelMenuClassic.rows.begin(), wheelMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelClassic", wheelMenuClassic});

		Menu wheelMenuWorn = {"Worn", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			wheelMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehicleWheelColor(vehicle, color.second);
			                                      }));
		}

		sort(wheelMenuWorn.rows.begin(), wheelMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelWorn", wheelMenuWorn});

		Menu wheelMenuUtils = {"Utils", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			wheelMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehicleWheelColor(vehicle, color.second);
			                                       }));
		}

		sort(wheelMenuUtils.rows.begin(), wheelMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelUtils", wheelMenuUtils});

		Menu wheelMenuMatte = {"Matte", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			wheelMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehicleWheelColor(vehicle, color.second);
			                                       }));
		}

		sort(wheelMenuMatte.rows.begin(), wheelMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelMatte", wheelMenuMatte});

		Menu wheelMenuMetals = {"Metals", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			wheelMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                        {
				                                        SetVehicleWheelColor(vehicle, color.second);
			                                        }));
		}

		sort(wheelMenuMetals.rows.begin(), wheelMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelMetals", wheelMenuMetals});

#pragma endregion

#pragma endregion

#pragma region Livery

		int liveryCount = VEHICLE::GET_VEHICLE_LIVERY_COUNT(vehicle);

		if (liveryCount != -1)
		{
			Menu liveryMenu = {"Livery" , "VehicleCustomisation", {}};

			for (int i = 0; i < liveryCount; i++)
			{
				liveryMenu.rows.push_back(enterrow(fmt::format("Livery {0}", i), [=]
				                                   {
					                                   SetVehicleLivery(vehicle, i);
				                                   }));
			}

			menus.push_back({"VehicleModLivery", liveryMenu});

			menu.rows.push_back(menurow("Livery", "VehicleModLivery"));
		}

#pragma endregion 

#pragma region Extras

		Menu extrasMenu = {"Extras" , "VehicleCustomisation", {}};

		for (int i = 1; i <= 12; i++) // 10-12 are mainly small interior features
		{
			if (VEHICLE::DOES_EXTRA_EXIST(vehicle, i))
			{
				extrasMenu.rows.push_back(enterrow(fmt::format("Toggle Extra {0}", i), [=]
				                                   {
					                                   ToggleExtra(vehicle, i);
				                                   }));
			}
		}

		if (extrasMenu.rows.size() > 0)
		{
			menus.push_back({"VehicleModExtra", extrasMenu});

			menu.rows.push_back(menurow("Extras", "VehicleModExtra"));
		}

#pragma endregion 

#pragma region Window Tint

		Menu windowTintMenu =
			{"Window Tint" , "VehicleCustomisation",
					{
						enterrow("Stock", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_STOCK);
						         }),
						enterrow("None", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_NONE);
						         }),
						enterrow("Light Smoke", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_LIGHTSMOKE);
						         }),
						enterrow("Dark Smoke", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_DARKSMOKE);
						         }),
						enterrow("Black", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_BLACK);
						         }),
						enterrow("Green", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_GREEN);
						         }),
					}
			};


		menus.push_back({"VehicleModWindowTint", windowTintMenu});

		menu.rows.push_back(menurow("Window Tint", "VehicleModWindowTint"));

#pragma endregion

#pragma region License Plate

		Menu licensePlateMenu =
			{"License Plate" , "VehicleCustomisation",
					{
						enterrow("Blue On White 1", [=]
						         {
							         SetPlateType(vehicle, PLATE_BLUEONWHITE1);
						         }),
						enterrow("Blue On White 1", [=]
						         {
							         SetPlateType(vehicle, PLATE_BLUEONWHITE2);
						         }),
						enterrow("Blue On White 1", [=]
						         {
							         SetPlateType(vehicle, PLATE_BLUEONWHITE3);
						         }),
						enterrow("Yellow On Blue", [=]
						         {
							         SetPlateType(vehicle, PLATE_YELLOWONBLUE);
						         }),
						enterrow("Yellow On Black", [=]
						         {
							         SetPlateType(vehicle, PLATE_YELLOWONBLACK);
						         }),
						enterrow("Yankton", [=]
						         {
							         SetPlateType(vehicle, PLATE_YANKTON);
						         }),
						inputrow("Set Number Plate", 8, [=] (char* result)
						         {
							         SetNumberPlate(vehicle, result);
						         }),
					}
			};

		menus.push_back({"VehicleModLicensePlate", licensePlateMenu});

		menu.rows.push_back(menurow("License Plate", "VehicleModLicensePlate"));

#pragma endregion

#pragma region Tire Smoke

		static int tireR, tireG, tireB;

		Menu tireSmokeMenu =
			{"Tire Smoke", "VehicleCustomisation",
					{
						introw("Tire Smoke Red", &tireR, 0, 255, 1),
						introw("Tire Smoke Green", &tireG, 0, 255, 1),
						introw("Tire Smoke Blue", &tireB, 0, 255, 1),
						enterrow("Get Tire Smoke from primary", [=]
								{
									tireR = primaryR;
									tireG = primaryG;
									tireB = primaryB;
								}),
						enterrow("Get Tire Smoke from secondary", [=]
								{
									tireR = secondaryR;
									tireG = secondaryG;
									tireB = secondaryB;
								}),
						enterrow("Set Tire Smoke", [&, vehicle]
						         {
							         SetTireSmoke(vehicle, tireR, tireG, tireB);
						         }),
					}
			};

		menus.push_back({"VehicleModTireSmoke", tireSmokeMenu});

		menu.rows.push_back(menurow("Tire Smoke", "VehicleModTireSmoke"));

#pragma endregion

#pragma region Neon Light

		static int neonR, neonG, neonB;

		Menu neonMenu =
			{"Neon Lights", "VehicleCustomisation",
					{
						enterrow("Toggle Front Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 2);
						         }),
						enterrow("Toggle Back Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 3);
						         }),
						enterrow("Toggle Left Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 0);
						         }),
						enterrow("Toggle Right Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 1);
						         }),
						introw("Neon Red", &neonR, 0, 255, 1),
						introw("Neon Green", &neonG, 0, 255, 1),
						introw("Neon Blue", &neonB, 0, 255, 1),
						enterrow("Get Neon color from primary", [=]
						{
							neonR = primaryR;
							neonG = primaryG;
							neonB = primaryB;
						}),
						enterrow("Get Neon color from secondary", [=]
						{
							neonR = secondaryR;
							neonG = secondaryG;
							neonB = secondaryB;
						}),
						enterrow("Set Neon Color", [&, vehicle]
						{
							VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, neonR, neonG, neonB);
						}),
					}
			};

		menus.push_back({"VehicleModNeon", neonMenu});

		menu.rows.push_back(menurow("Neon Lights", "VehicleModNeon"));

#pragma endregion

#pragma region Vehicle Toggle Mods

		for each (std::pair<std::string, int> toggle in VehicleModToggles)
		{	

			char* key = strchr(fmt::format("VehicleToggle{0}", toggle.first));

			//menu.rows.push_back(enterrow(fmt::format("Toggle {0}", toggle.first), [=]
			//                             {
			//	                             ToggleVehicleMod(vehicle, toggle.second);
			//                             }));

			Menu toggleMenu = { strchr(toggle.first) , "VehicleCustomisation",{} };

			std::map<int, char*> predefined = manualStrings[toggle.second];

			toggleMenu.rows.push_back(enterrow(predefined[-1] == nullptr ? "Default" : std::string(predefined[-1]), [=]
			{
				SetVehicleToggle(vehicle, toggle.second, FALSE);
			}));

			char* modCodeName = VEHICLE::GET_MOD_TEXT_LABEL(vehicle, toggle.second, 0);
			char* modDisplayName = UI::_GET_LABEL_TEXT(modCodeName);

				if (AreStringsEqual(modDisplayName, "NULL"))
				{
					if (predefined[0] != nullptr)
					{
						modDisplayName = predefined[0];
					}
					else
					{
						modDisplayName = strchr(fmt::format("{0} {1}", toggle.first, 0));
					}
				}

				toggleMenu.rows.push_back(enterrow(std::string(modDisplayName), [=]
				{
					SetVehicleToggle(vehicle, toggle.second, TRUE);
				}));				

			menu.rows.push_back(menurow(toggle.first, key));

			menus.push_back({ key, toggleMenu });
		}

#pragma endregion

		sort(menus.begin(), menus.end(), MenuPairSort);
	}

	menus.push_back({"VehicleCustomisation", menu});

	return menus;
}

color_t makeColorGradient(int i, double frequency1, double frequency2, double frequency3 ,
	double phase1 , double phase2 , double phase3 ,
	int center, int width, int len)
{
	if (center == 0)   center = 128;
	if (width == 0)    width = 127;
	if (len == 0)      len = 44;
	int c = i;
	if (c >= len)
		c %= len;
	color_t res;
	res.red = sin(frequency1*c + phase1) * (double)width + (double)center;
	res.green = sin(frequency2*c + phase2) * (double)width + (double)center;
	res.blue = sin(frequency3*c + phase3) * (double)width + (double)center;
	return res;
}
int getClosestStandartColor(color_t * col) {
	double distance = 250000.0;
	int res = 0;
	for each(std::pair<int,color_t> o in standartColorsRGB)
	{
		double dbl_test_red = pow(o.second.red - col->red, 2.0);
		double dbl_test_green = pow(o.second.green - col->green, 2.0);
		double dbl_test_blue = pow(o.second.blue - col->blue, 2.0);
		
		double temp = sqrt(dbl_test_blue + dbl_test_green + dbl_test_red);
		// explore the result and store the nearest color
		if (temp == 0.0)
		{
			return o.first;
		}
		else if (temp < distance)
		{
			distance = temp;
			res = o.first;
		}
	}
	return res;
}

void SetOffAlarmofPlayerVehicle(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Entity selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		RequestControl(PED::GET_VEHICLE_PED_IS_USING(selectedPed));
		VEHICLE::SET_VEHICLE_ALARM(selectedVehicle, true);
		VEHICLE::START_VEHICLE_ALARM(selectedVehicle);
		DrawNotification("Set off alarm of vehicle");
	}
	else
		DrawNotification("Player isn't in a vehicle");
}


void AIJackVehicle(Ped selectedPlayer)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, FALSE))
	{
		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);

		//Remove PED from vehicle
		AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPlayer);
		//cloning player
		Ped Driver = PED::CLONE_PED(selectedPlayer, 0.0f, false, false);
		//Ped Driver = PED::CLONE_PED(selectedPlayer, 0.0f, true,true);
		PED::ADD_ARMOUR_TO_PED(Driver, 1000);
		PED::SET_PED_DIES_WHEN_INJURED(Driver, false);
		PED::SET_PED_CAN_BE_SHOT_IN_VEHICLE(Driver, false);
		PED::SET_PED_CAN_BE_TARGETTED(Driver, false);
		PED::SET_PED_CAN_BE_DRAGGED_OUT(Driver, false);
		AI::TASK_ENTER_VEHICLE(Driver, playerVeh, 100, SEAT_DRIVER, 2.0f, 1, 0);
		//PED::SET_PED_INTO_VEHICLE(Driver, playerVeh, SEAT_DRIVER);

		//AI::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Driver, true);
		//WAIT(5000);
		AI::TASK_VEHICLE_DRIVE_TO_COORD(selectedPlayer, playerVeh, -2000.0f, -1000.0f, 0.0f, 100.0f, 1, ENTITY::GET_ENTITY_MODEL(playerVeh), 0, 0xC00AB, -1);
	}
	else
		DrawNotification("Player isn't in a vehicle");
}

Vehicle spawnMoneyVehicle(Ped selfPed, Ped selectedPlayer)
{
	Hash faggio = $("BUFFALO");
	STREAMING::REQUEST_MODEL(faggio);
	while (!STREAMING::HAS_MODEL_LOADED(faggio)) WAIT(0);
	Vector3 endPosition = ENTITY::GET_ENTITY_COORDS(selectedPlayer,true);
	Vehicle car = VEHICLE::CREATE_VEHICLE(faggio, endPosition.x, endPosition.y, endPosition.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), true, true);
	if(NETWORK::NETWORK_IS_IN_SESSION())
		RequestControl(car);
	ENTITY::SET_ENTITY_VISIBLE(car, false, false);
	VEHICLE::_SET_CAR_CREATES_MONEY_PICKUPS_WHEN_EXPLODES(car, true);
	WAIT(0);
	VEHICLE::EXPLODE_VEHICLE(car, true, false);
	WAIT(0);
	ENTITY::SET_ENTITY_COLLISION(car, false, false);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(car, 1, 1);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(car, -1494, 8500, 9000, 0, 0, 1);
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&car);
	ENTITY::DELETE_ENTITY(&car);
	return car;
}
