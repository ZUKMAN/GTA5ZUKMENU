#include "stdafx.h"
#pragma region Config

static float MenuX = 0.75f;
static float MenuY = 0.0625f;
static float MenuWidth = 0.2125f;
static float MenuRight = MenuX + MenuWidth;
static float RowHeight = 0.0325f;
static float TitleHeight = RowHeight * 2.5f;
static float MaxHeight = 1.0f - TitleHeight - MenuY * 2.0f;

static int MaxRows = int(round(1 / RowHeight * MaxHeight) - 1);
static int Middle = int(floor(MaxRows * 0.5f - 0.5f)); 
// We wanna make sure it's BEFORE half way, if MaxRows is even

// The only useful fonts are 0,1,4 and 7 (maybe 3 if you need some symbols, see Useful Files\fonts2.png)

static color_t DefaultTextColor = { 240, 240, 240, 255 };
static color_t DefaultRectColor = { 0,154,167, 150 };
static float DefaultTextScale = 0.45f;
static int DefaultFont = 0;
static Alignment DefaultAlignment = Left;

static color_t TitleTextColor = { 255, 255, 255, 255 };
static color_t TitleRectColor = { 0,154,167, 255 };
static float TitleTextScale = 0.8f;
static int TitleFont = 7;
static Alignment TitleAlignment = Center;

static color_t ActiveTextColor = DefaultTextColor;
static color_t ActiveRectColor = { 0, 154,167, 50 };
static float ActiveTextScale = DefaultTextScale;
static int ActiveFont = DefaultFont;
static Alignment ActiveAlignment = DefaultAlignment;

static color_t OnTextColor = { 150, 154,167, DefaultTextColor.alpha };
static color_t OffTextColor = { 50, 154,167, DefaultTextColor.alpha };
static color_t IntTextColor = { 100, 154,167, DefaultTextColor.alpha };
static color_t MenuTextColor = { 100, 154,167, DefaultTextColor.alpha };

static float TextMargin = 0.005f;
static float LineHeight = 0.002f;

static char* SoundSet = "HUD_FRONTEND_DEFAULT_SOUNDSET"; // HUD_FRONTEND_DEFAULT_SOUNDSET (recommended) or HUD_FREEMODE_SOUNDSET

static bool menuVisible; // Menu Visible and Key Presses
static bool initialized;

static Menu currMenu;
static int currentSelectionIndex = 0;
static std::vector<Vehicle> toRemoveVehicles;
#pragma endregion


void UpdateMenu()
{
	#pragma region Row Variables
	static SavedOutfit * currentOutfit = new SavedOutfit();
	static Hash * originalPedModel = new Hash(), *currentPedModel = new Hash();
	static bool
		spawnInVehicle = true,
		baseUpgrades = true,
		maxUpgrades,
		featureVehSlammed,
		preventKick,
		customBounty,
		enableSnow,
		enableRace,
		christmasContent,
		valentinesContent,
		independenceContent,
		sellCap,
		fireworksLauncher,
		playerGodMode,
		vehicleGodMode,
		vehicleRandomColor,
		vehicleRainbowColor,
		vehicleRainbowColorPastel,
		vehicleSeatBelt,
		disableRagdoll,
		autoFixOutFit = false,
		richPeds,
		robPeds,
		neverWanted,
		nightVision,
		heatVision,
		rpLoop,
		superBunker2,
		frezeTime,
		disableRadio,
		skinchanger_used,
		enableWalkRadio,
		infiniteAmmo,
		explosiveAmmo,
		objectRemoveGun,
		showDamagedBone,
		oneHitKill,
		lazerSight,
		playerAimbot,
		pedAimbot,
		aimbotTrace,
		legitAimbotSpray,
		randomBoneAimbot,
		playerTriggerbot,
		pedTriggerbot,
		playerInvisible,
		fastSwim,
		radarOff,
		fastRun,
		disableCollission = false,
		featurePlayerESP,
		featurePlayerESPBoxes,
		featurePedSkeleton,
		featurePlayerSkeleton,
		featurePlayerESPTrace,
		syncRainbowcolors,
		syncRainbowcolorsNeon,
		syncRainbowcolorsTyre,
		syncRainbowPerl,
		syncRainbowInt,
		syncRainbowDash,
		syncRainbowWheels,
		fixOutfit = false,
		fixModel = false,
		disableIdleKick = true,
		carMoneyGun = false,
		carMoneyGunDrop = false,
		flyToggle = false;
	static BOOL spectateCam;
	static DWORD vehicleUpdateTime, aimbotShootTimer, aimbotShootTimerAdvance, pedAimbotShootTimer , pedAimbotShootTimerAdvance, outfitUpdateTime, riotPedTimer,objectRemoveTimer;
	static int vehicleColorSineindex = 0;
	static int rpmMultipler = 1, torqueMultiplier = 1, currTunable, gbRewardMultiplier = 1, headOverOpacity =100;
	static int fHour, fMinute, fsecond;
	static std::map<char*, int> prevSelections;
	static float yaw;

	static std::vector<SavedVehicle> savedVehicles;
	static std::vector<SavedOutfit> savedOutfits;
	static std::vector<int> aimbotBones;
	static Player currentPlayer = 0;

	#pragma endregion 
	Player selfPlayer = PLAYER::PLAYER_ID();
	Ped selfPed = PLAYER::PLAYER_PED_ID();
	Vehicle selfVehicle = PED::GET_VEHICLE_PED_IS_IN(selfPed, FALSE);	
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(selfPed, TRUE);
	Vector3 backTpCoords;
	/*
		For a list of font colors (e.g ~HUD_COLOUR_GREEN~) see 'Useful Files\hudcolor.dat'

		[]: no variables defined. Attempting to use any external variables in the lambda is an error.
		[x, &y]: x is captured by value, y is captured by reference
		[&]: any external variable is implicitly captured by reference if used
		[=]: any external variable is implicitly captured by value if used
		[&, x]: x is explicitly captured by value. Other variables will be captured by reference
		[=, &z]: z is explicitly captured by reference. Other variables will be captured by value
	*/

	if(currentOutfit==NULL)
		currentOutfit = new SavedOutfit();
	#pragma region Menu

	static std::map<std::string, Menu> Menus =
	{
		{"Main", // Key
			//{"Brick's Menu", "Main", // Title, Key for Previous Menu
			{"Main Menu", "Main", // Title, Key for Previous Menu
				{
					menurow("Self", "Self"),
					menurow("Money", "Money"),
					menurow("Teleport", "Teleport"),
					menurow("Weapons", "Weapon"),
					menurow("Vehicle", "Vehicle"),
					menurow("Online", "Online"),
					menurow("Tunables", "Tunables"),
					menurow("Misc",	"Misc"),
					menurow("Objects", "Objects"),
				}
			}
		},
		{"Self",
			{"Self", "Main",
				{
					dynmenurow("Change model", "ChangeModel", [&] { Menus["ChangeModel"] = ModelMenu(/*originalPedModel,currentPedModel*/); }),
					boolrow("Autofix outfit",		&autoFixOutFit),
					inputrow("Save Current outfit", 16, [&](char* result) { { savedOutfits.push_back(SaveOutfit(result, selfPed)); } }),
					dynmenurow("Saved Outfits", "OutfitSaved", [&] { Menus["OutfitSaved"] = SavedOutfitMenu(savedOutfits, selfPed, currentOutfit); }),
					enterrow("Save outfits to disk",	[&] { SaveOutfitsToDisk(savedOutfits); }),
					enterrow("Load outfits from disk",	[&] { savedOutfits = LoadOutfitsFromDisk(); Menus["OutfitSaved"] = SavedOutfitMenu(savedOutfits, selfPed, currentOutfit); }),
					dynmenurow("Clothing", "Clothing", [&] { for each (std::pair<std::string, Menu> menu in ClothingMenus(selfPed,NULL,&headOverOpacity)) { Menus[menu.first] = menu.second; } }),
					dynmenurow("Overlays", "Overlays", [&] { for each (std::pair<std::string, Menu> menu in OverlayMenus(selfPed,NULL)) { Menus[menu.first] = menu.second; } }),
					//dynmenurow("Facial Overlays", "Facial Overlays", [&] { for each (std::pair<std::string, Menu> menu in OverlayMenus(selfPed,true)) { Menus[menu.first] = menu.second; } }),
					menurow("Visions", "Vision"),
					boolfuncrow("God Mode",			&playerGodMode,[&] {ENTITY::SET_ENTITY_INVINCIBLE(selfPed, playerGodMode);
																		ENTITY::SET_ENTITY_PROOFS(selfPed, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode);}),
					boolfuncrow("Disable Ragdoll",		&disableRagdoll,[&] {PED::SET_PED_CAN_RAGDOLL(selfPed, !disableRagdoll);}),
					boolrow("Never Wanted",		&neverWanted),
					enterrow("Replenish Self",		[&] { ReplenishPlayer(selfPed);			}),
					boolfuncrow("Invisible", &playerInvisible, [] { setPlayerVisible(PLAYER::PLAYER_PED_ID(), !playerInvisible); }),
					boolrow("Fast swim", &fastSwim),
					boolrow("Fast run",  &fastRun),
					boolfuncrow("Disable collision",  &disableCollission,[&] {ENTITY::SET_ENTITY_COLLISION(selfPed, !disableCollission, TRUE);}),
					enterrow("Clear Wanted Level",	[&] { ClearWantedLevel(selfPlayer);		}),
					enterrow("Wanted Level Up",     [&] { wantedLevelUp(selfPlayer ,selfPed); }),
					enterrow("Wanted Level Down",   [&] { wantedLevelDown(selfPlayer, selfPed); }),		
				}
			}
		},
		{ "Objects",
			{ "Objects", "Main",
				{
					inputrow("Spawn prop", 50, [&](char* result) {
						Hash beach_fire = GAMEPLAY::GET_HASH_KEY(result);
						STREAMING::REQUEST_MODEL(beach_fire);
						while (!STREAMING::HAS_MODEL_LOADED(beach_fire)) WAIT(0);
						Vector3 selfPos = ENTITY::GET_ENTITY_COORDS(selfPed, true);
						Object object = OBJECT::CREATE_OBJECT(beach_fire, selfPos.x, selfPos.y, selfPos.z, true, true, false);
					}),
					enterrow("10 garage troll",   [] { 
						Hash beach_fire = GAMEPLAY::GET_HASH_KEY("prop_windmill_01");
						STREAMING::REQUEST_MODEL(beach_fire);
						while (!STREAMING::HAS_MODEL_LOADED(beach_fire)) WAIT(0);
						Object object = OBJECT::CREATE_OBJECT(beach_fire, 230.7995f, -1016.485f, -96.71713f, true, true, false);
						RequestControl(object);
						ENTITY::SET_ENTITY_COORDS(object, 230.7995f, -1016.485f, -96.71713f, 0, 0, 0, 1);
						ENTITY::SET_ENTITY_ROTATION(object, 85.00011f, 85.00011f, 89.99986f, 2, 1);
						ENTITY::FREEZE_ENTITY_POSITION(object, 1);
						ENTITY::SET_ENTITY_LOD_DIST(object, 696969);
						ENTITY::SET_ENTITY_INVINCIBLE(object, TRUE);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(beach_fire);
					}),
					enterrow("Main LSC troll",   [] {
						Hash beach_fire = GAMEPLAY::GET_HASH_KEY("prop_windmill_01");
						STREAMING::REQUEST_MODEL(beach_fire);
						while (!STREAMING::HAS_MODEL_LOADED(beach_fire)) WAIT(0);
						Object object = OBJECT::CREATE_OBJECT_NO_OFFSET(beach_fire, -412.208f, -113.816f, 35.6642f, true, true, false);
						RequestControl(object);
						ENTITY::SET_ENTITY_ROTATION(object, -89.5252f, 0, -110.115f, 2, 1);
						ENTITY::FREEZE_ENTITY_POSITION(object, 1);
						ENTITY::SET_ENTITY_LOD_DIST(object, 696969);
						ENTITY::SET_ENTITY_INVINCIBLE(object, TRUE);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(beach_fire);
					}), 
					enterrow("Benny's troll",   [] {
						Hash beach_fire = GAMEPLAY::GET_HASH_KEY("prop_windmill_01");
						STREAMING::REQUEST_MODEL(beach_fire);
						while (!STREAMING::HAS_MODEL_LOADED(beach_fire)) WAIT(0);
						Object object = OBJECT::CREATE_OBJECT(beach_fire, -207.634f, -1338.185, 33.71222f, true, true, false);
						RequestControl(object);
						ENTITY::SET_ENTITY_COORDS(object, -207.634f, -1338.185, 33.71222f, 0, 0, 0, 1);
						ENTITY::SET_ENTITY_ROTATION(object, -89.5252f, 0, -110.115f, 2, 1);
						ENTITY::FREEZE_ENTITY_POSITION(object, 1);
						ENTITY::SET_ENTITY_LOD_DIST(object, 696969);
						ENTITY::SET_ENTITY_INVINCIBLE(object, TRUE);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(beach_fire);
					}),
				
				}
			}
		},
		{ "Money",
			{ "Money", "Main",
				{
					//introw("GB Miltiplier",	&gbRewardMultiplier, 1, 100, 1),
					enterrow("Reset GB last time", [&] {
						int * res = new int();
						*res = 0;
						STATS::STAT_GET_INT($("MP0_GB_LAST_TIME_AS_BOSS"), res, 0);
						*res -= (12 * 60 * 60 * 1000);
						STATS::STAT_SET_INT($("MP0_GB_LAST_TIME_AS_BOSS"), *res, TRUE);
						res = new int();
						*res = 0;
						STATS::STAT_GET_INT($("MP1_GB_LAST_TIME_AS_BOSS"), res, 0);
						*res -= (12 * 60 * 60 * 1000);
						STATS::STAT_SET_INT($("MP1_GB_LAST_TIME_AS_BOSS"), *res, TRUE);
					}),
					enterrow("Reset GB JOB timer",	[&] {
						int val = 1;
						STATS::STAT_SET_INT($("MP0_GB_ASSAULT_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP0_GB_BELLY_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP0_GB_BOSS_DM_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP0_GB_HUNT_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP0_GB_SEER_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP0_GB_YACHT_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP1_GB_ASSAULT_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP1_GB_BELLY_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP1_GB_BOSS_DM_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP1_GB_HUNT_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP1_GB_SEER_CD"), val, TRUE);
						STATS::STAT_SET_INT($("MP1_GB_YACHT_CD"), val, TRUE);
					}),		
					boolrow("Rich peds",  &richPeds),
					boolrow("Rob peds",  &robPeds),
				}
			}
		},
		{ "Teleport",
			{ "Teleport", "Main",
				{
					enterrow("Marker",	[&] { 
						backTpCoords =  ENTITY::GET_ENTITY_COORDS(PED::IS_PED_IN_ANY_VEHICLE(selfPed, TRUE) ? PED::GET_VEHICLE_PED_IS_USING(selfPed): selfPed,true);
						teleport(true, vector3_t()); 
					}),
					enterrow("Mission Objective",  [&] { 
						backTpCoords = ENTITY::GET_ENTITY_COORDS(PED::IS_PED_IN_ANY_VEHICLE(selfPed, TRUE) ? PED::GET_VEHICLE_PED_IS_USING(selfPed) : selfPed,true);
						MissionObjective(selfPed); 
					}),
					enterrow("Teleport to Nearest Car", [&] { toNearestCar(selfPed); }),
					enterrow("Teleport Back",	[&] {
						vector3_t coords;
						coords.x = backTpCoords.x;
						coords.y = backTpCoords.y;
						coords.z = backTpCoords.z;
						teleport(false, coords);
					}),
					enterrow("Personal vehicle",  [&] {
					if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BlipSpritePersonalVehicleCar)))
						TeleportToBlip(selfPed, BlipSpritePersonalVehicleCar);
					else
						if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BlipSpritePersonalVehicleBike)))
							TeleportToBlip(selfPed, BlipSpritePersonalVehicleBike);
						else
							DrawNotification("No PV");
					}),
					enterrow("Ammunation",  [&] {
						int blipSprice = BlipSpriteAmmuNation;
						if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(blipSprice)))
							TeleportToBlip(selfPed, blipSprice);
						else
							DrawNotification("Failed");
					}),
					enterrow("Clothing store",  [&] {
						int blipSprice = BlipSpriteClothes;
						if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(blipSprice)))
							TeleportToBlip(selfPed, blipSprice);
						else
							DrawNotification("Failed");
					}),
					enterrow("Food store",  [&] {
						int blipSprice = BlipSpriteStore;
						if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(blipSprice)))
							TeleportToBlip(selfPed, blipSprice);
						else
							DrawNotification("Failed");
					}),
					enterrow("Chauffeur Drive to Waypoint/Cancel", [&] {Chauffeur(selfPed); }),
					dynmenurow("Locations", "Location", [&] { Menus["Location"] = TpLocationsMenu(); }),
				}
			}
		},
		{ "Weapon",
			{ "Weapon", "Main",
				{
					enterrow("Give All Weapons",	[&] { GiveAllWeapons(selfPed);			}),
					boolrow("Infinite Ammo",		&infiniteAmmo),
					boolrow("Explosive Ammo",        &explosiveAmmo),
					boolrow("Player aimbot", &playerAimbot),
					boolrow("Player triggerbot", &playerTriggerbot),
					boolrow("Ped aimbot", &pedAimbot),
					boolrow("Ped triggerbot", &pedTriggerbot),
					boolrow("Bullet trace", &aimbotTrace),
					boolrow("Legit spray", &legitAimbotSpray),
					boolrow("Shoot random bone", &randomBoneAimbot),
					boolrow("Object remove gun", &objectRemoveGun),
				}
			}
		},
		{"Vision",
			{"Vison", "Self",
				{
					boolfuncrow("Night Vision", &nightVision, [] { GRAPHICS::SET_NIGHTVISION(nightVision); }),
					boolfuncrow("Heat Vision", &heatVision, [] { GRAPHICS::SET_SEETHROUGH(heatVision); }),
					boolrow("ESP",		&featurePlayerESP),
					boolrow("ESP Boxes",		&featurePlayerESPBoxes),
					boolrow("ESP Skeleton ped",		&featurePedSkeleton),
					boolrow("ESP Skeleton player",		&featurePlayerSkeleton),
					boolrow("ESP Trace",		&featurePlayerESPTrace),
				}
			}
		},
		{"Vehicle",
			{"Vehicle", "Main",
				{
					menurow("Spawn Vehicle",	"VehicleSpawn"),
					dynmenurow("Saved Vehicles", "VehicleSaved", [&] { Menus["VehicleSaved"] = SavedMenu(savedVehicles, selfPed, &spawnInVehicle,preventKick); }),
					enterrow("Save vehicles to disk",	[&] { SaveVehiclesToDisk(savedVehicles); }),
					enterrow("Load vehicles from disk",	[&] { savedVehicles = LoadVehiclesFromDisk(); Menus["VehicleSaved"] = SavedMenu(savedVehicles, selfPed, &spawnInVehicle,preventKick); }),
					dynmenurow("Customization", "VehicleCustomisation", [&] { for each (std::pair<std::string, Menu> menu in VehicleCustomMenu(selfVehicle)) { Menus[menu.first] = menu.second; } }),
					menurow("Spawn settings",	"CarSpawnSettings"),
					enterrow("Repair Vehicle",	[&] { RepairVehicle(selfVehicle); }),
					enterrow("Base Upgrades",	[&] { BoostBaseVehicleStats(selfVehicle, true); }),
					enterrow("Max Upgrades",	[&] { MaxUpgrade(selfVehicle); }),
					menurow("Randomize color",	"CarRandomColor"),
					boolrow("Seat Belt",		&vehicleSeatBelt),
					boolrow("God Mode",			&vehicleGodMode),
					boolrow("Slammed",			&featureVehSlammed),
					introw("RPM Multiplier",	&rpmMultipler, 1, 100, 1),
					introw("Torque Multiplier", &torqueMultiplier, 1, 100, 1),
					boolfuncrow("Car fly",  &flyToggle,[&] { yaw = ENTITY::GET_ENTITY_HEADING(selfPed);
															//The gravity will stop the car from "dragging" down slowly.
															VEHICLE::SET_VEHICLE_GRAVITY(selfVehicle, !flyToggle);
					}),
					inputrow("Save Current Vehicle", 16, [&](char* result) { if (PED::IS_PED_IN_ANY_VEHICLE(selfPed, FALSE)) { savedVehicles.push_back(SaveVehicle(result, selfVehicle)); } }),
				}
			}
		},
		{ "CarSpawnSettings",
			{ "Spawn settings", "Vehicle",
				{
					boolrow("Spawn In Vehicle", &spawnInVehicle),
					boolrow("Prevent kick",     &preventKick),
					boolrow("Base Upgrades",	&baseUpgrades),
					boolrow("Max Upgrades",		&maxUpgrades),
				}
			}
		},
		{ "CarRandomColor",
			{ "Randomize color", "Vehicle",
				{
					boolrow("Random color",		&vehicleRandomColor),
					boolrow("Rainbow color",	&vehicleRainbowColor),
					boolrow("Rainbow color pastel",	&vehicleRainbowColorPastel),
					boolrow("Sync rainbow secondary",	&syncRainbowcolors),
					boolrow("Sync rainbow neon",	&syncRainbowcolorsNeon),
					boolrow("Sync rainbow tyre smoke",	&syncRainbowcolorsTyre),
					boolrow("Sync wheels color",	&syncRainbowWheels),
					boolrow("Sync perl color",	&syncRainbowPerl),
					boolrow("Sync interior color",	&syncRainbowInt),
					boolrow("Sync dashboard color",	&syncRainbowDash),
				}
			}
		},
		{"VehicleSpawn",
			{"Vehicle Spawn", "Vehicle",
				{
					menurow("Airplanes",		"SpawnAirplane"),
					menurow("Bicycles",			"SpawnBicycle"),
					menurow("Boats",			"SpawnBoats"),
					menurow("Commercial",		"SpawnCommercial"),
					menurow("Compacts",			"SpawnCompacts"),
					menurow("Coupes",			"SpawnCoupes"),
					menurow("Emergency",		"SpawnEmergency"),
					menurow("Helicopters",		"SpawnHelicopter"),
					menurow("Industrial",		"SpawnIndustrial"),
					menurow("Motorcycles",		"SpawnMotorcycle"),
					menurow("Military",			"SpawnMilitary"),
					menurow("Muscle",			"SpawnMuscle"),
					menurow("Off-Road",			"SpawnOffroad"),
					menurow("Sedans",			"SpawnSedans"),
					menurow("Service",			"SpawnService"),
					menurow("Sports",			"SpawnSports"),
					menurow("Sports Classics",	"SpawnSportsClassics"),
					menurow("Super",			"SpawnSuper"),
					menurow("SUVs",				"SpawnSUVs"),
					menurow("Utility",			"SpawnUtility"),
					menurow("Vans/Pickups",		"SpawnVansPickups"),
					menurow("Others",			"SpawnOther"),
					inputrow("Input Name", 20, [&] (char* result) { if (DoesVehicleExist(result)) { SpawnPlayerVehicle(selfPed, $(result), spawnInVehicle, baseUpgrades, maxUpgrades,preventKick); } else { DrawNotification("Invalid Vehicle Name"); } }),
				}
			}
		},
		{ "Online",
			{ "Online", "Main",
				{
				}
			}
		},
		{ "Selected Player",
			{ PLAYER::GET_PLAYER_NAME(currentPlayer), "Online",
				{
					menurow("Troll options",	"Troll options"),
					enterrow("Teleport To", [] { TeleportToClient(PLAYER::GET_PLAYER_PED(currentPlayer), currentPlayer); }),
					enterrow("Teleport To Me", [] { TeleportToMe(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED(currentPlayer)); }),
					boolrow("Car money drop", &carMoneyGunDrop),
					enterrow("Frame", [] { FramePlayer(PLAYER::GET_PLAYER_PED(currentPlayer)); }),
					//enterrow("Crash Player", [] { crashPlayer(PLAYER::GET_PLAYER_PED(currentPlayer), PLAYER::PLAYER_PED_ID()); }),
					//enterrow("Clear wanted level", [] { 
					//	PLAYER::CLEAR_PLAYER_WANTED_LEVEL(currentPlayer);
					//	PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(currentPlayer, false);
					////}),
					/*
					enterrow("Add wanted level", [] {
						PLAYER::REPORT_CRIME(currentPlayer, 36, PLAYER::GET_WANTED_LEVEL_THRESHOLD(4));
						PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(currentPlayer, false);
					}),
					*/
					enterrow("Give All Weapons", [] { GiveAllWeapons(PLAYER::GET_PLAYER_PED(currentPlayer)); }),
					enterrow("Replenish player", [] { ReplenishPlayer(PLAYER::GET_PLAYER_PED(currentPlayer)); }),
					enterrow("Send car flying", [] {
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(currentPlayer), TRUE))
						{
							Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(currentPlayer), FALSE);
							if(ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(vehicle))
								ENTITY::DETACH_ENTITY(vehicle, true, true);
							Hash pHash = ENTITY::GET_ENTITY_MODEL(vehicle);
							if (VEHICLE::IS_THIS_MODEL_A_CAR(pHash) ||
								VEHICLE::IS_THIS_MODEL_A_BIKE(pHash) || VEHICLE::IS_THIS_MODEL_A_TRAIN(pHash) || 
								VEHICLE::IS_THIS_MODEL_A_BICYCLE(pHash) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(pHash)) {
								
								Hash skylift = GAMEPLAY::GET_HASH_KEY("SKYLIFT");
								STREAMING::REQUEST_MODEL(skylift);
								while (!STREAMING::HAS_MODEL_LOADED(skylift)) WAIT(0);
								
								Vector3 vehCoords = ENTITY::GET_ENTITY_COORDS(vehicle, true);
								Vehicle pMyVehicle = VEHICLE::CREATE_VEHICLE(skylift, vehCoords.x, vehCoords.y, vehCoords.z+3,ENTITY::GET_ENTITY_HEADING(vehicle),true,true);
								RequestControl(pMyVehicle);
								ENTITY::SET_ENTITY_LOD_DIST(pMyVehicle, 696969);
								ENTITY::SET_ENTITY_INVINCIBLE(pMyVehicle, TRUE);
								ENTITY::SET_ENTITY_VISIBLE(pMyVehicle, 0, true);
								RequestControl(vehicle);
								ENTITY::ATTACH_ENTITY_TO_ENTITY(vehicle, pMyVehicle, 0, 0.0, -3.0, -1.0, 0.0, 0.0, 0.0, true, true, true, true, 1, true);
								applyForceToEntity(pMyVehicle, 0.0f, 0.0f, 5000.0f);
								WAIT(1000);
								ENTITY::DETACH_ENTITY(vehicle, true, true);
								ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pMyVehicle, 1, 1);
								ENTITY::SET_ENTITY_COORDS_NO_OFFSET(pMyVehicle, -1494, 8500, 9000, 0, 0, 1);
								ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&pMyVehicle);
								ENTITY::DELETE_ENTITY(&pMyVehicle);
								STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(skylift);
							}

						}
					}),
					
					
					enterrow("AIJack player's vehicle", [] {
						AIJackVehicle(PLAYER::GET_PLAYER_PED(currentPlayer));
					}),
					
					//dynmenurow("Clothing", "Clothing", [&] { for each (std::pair<std::string, Menu> menu in ClothingMenus(PLAYER::GET_PLAYER_PED(currentPlayer),currentPlayer)) { Menus[menu.first] = menu.second; } }),
					inputrow("Save Current outfit(steal)", 16, [&](char* result) { { savedOutfits.push_back(SaveOutfit(result,PLAYER::GET_PLAYER_PED(currentPlayer))); } }),
					dynmenurow("Saved Outfits", "OutfitSavedPlayer", [&] { Menus["OutfitSavedPlayer"] = SavedOutfitMenu(savedOutfits, PLAYER::GET_PLAYER_PED(currentPlayer), NULL); }),
					inputrow("Save Current Vehicle", 16, [&](char* result) { 
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(currentPlayer), FALSE)) 
						{
							savedVehicles.push_back(SaveVehicle(result, PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(currentPlayer))));
						} 
					}),
					//dynmenurow("Saved Vehicles", "VehicleSavedPlayer", [&] { Menus["VehicleSavedPlayer"] = SavedMenu(savedVehicles, PLAYER::GET_PLAYER_PED(currentPlayer), &spawnInVehicle,preventKick); }),
					
				
					enterrow("Kick(Host Only)", [] { KickPlayer(currentPlayer); }),
					enterrow("Spectate ", [&] { spectateCam = SpectatePlayer(PLAYER::GET_PLAYER_PED(currentPlayer)); }),
					//boolrow("Freeze",      &freeze),
					//boolrow("Spawn Money Loop", &dropMoneyOnline),
				}
			}
		},
		
		{ "Troll options",
			{ "Troll options", "Selected Player",
				{
					enterrow("Random ped attack", [] {
						AttackPlayerWithRandomPeds(PLAYER::GET_PLAYER_PED(currentPlayer));
					}),
					enterrow("Explode Player", [] { ExplodePlayer(PLAYER::GET_PLAYER_PED(currentPlayer), currentPlayer); }),
					enterrow("Burst tyres", [] {
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(currentPlayer), TRUE))
						{
							Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(currentPlayer), FALSE);
							RequestControl(selectedVehicle);
							VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(selectedVehicle, TRUE);
							static int tireID = 0;
							for (tireID = 0; tireID < 8; tireID++)
								VEHICLE::SET_VEHICLE_TYRE_BURST(selectedVehicle, tireID, true, 1000.0);

						}
						else
						{
							DrawNotification("Player isn't in a vehicle");
						}
					}),
					enterrow("SEt off car alarm", [] {
						SetOffAlarmofPlayerVehicle(PLAYER::GET_PLAYER_PED(currentPlayer));
					}),
					enterrow("Kick Out From Vehicle", [] { KickOutFromVehicle(PLAYER::GET_PLAYER_PED(currentPlayer)); }),
					enterrow("Remove All Weapons", [] { WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED(currentPlayer), TRUE); }),
					enterrow("Trap", [] {
						Ped selectedPed = PLAYER::GET_PLAYER_PED(currentPlayer);
						if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
						{
							Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
							RequestControl(selectedVehicle);
							AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
							VEHICLE::SET_VEHICLE_DOORS_LOCKED_FOR_ALL_PLAYERS(selectedVehicle, true);
						}
						AI::CLEAR_PED_SECONDARY_TASK(selectedPed);
						Vector3 pedLocation = ENTITY::GET_ENTITY_COORDS(selectedPed,1);
						Hash cableCar = $("p_cablecar_s");
						STREAMING::REQUEST_MODEL(cableCar);
						while (!STREAMING::HAS_MODEL_LOADED(cableCar)) WAIT(0);
						Object cb1 = OBJECT::CREATE_OBJECT(cableCar, pedLocation.x, pedLocation.y, pedLocation.z-2, 1, 1, 0);
						ENTITY::SET_ENTITY_HEADING(cb1, 0);
						ENTITY::FREEZE_ENTITY_POSITION(cb1, 1);
						Object cb2 = OBJECT::CREATE_OBJECT(cableCar, pedLocation.x, pedLocation.y, pedLocation.z-2, 1, 1, 0);
						ENTITY::SET_ENTITY_HEADING(cb1, 90);
						ENTITY::FREEZE_ENTITY_POSITION(cb2, 1);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(cableCar);
					}),
				}
			}
		},
		/*
		#pragma region tunables
		{"Tunables",
			{"Tunables", "Main",
				{
					boolfuncrow("Disable Idle Kick",	&disableIdleKick,[] {} ),
					//boolfuncrow("Disable Idle Kick",	&disableIdleKick,[] {}),
				}
			}
		},
		*/
		#pragma endregion
		
		/*
		{"Recovery", "Main",
				{
					boolrow("RP Loop", &rpLoop),
				}
			}
		},*/	
		
{"Misc",
			{"Miscellaneous", "Main",
				{
					menurow("Time",	"SetTime"),
					menurow("Weather","SetWeather"),
					menurow("Radio","ToggleRadio"),
					enterrow("Reset",		   [] { initialized = false; }),
					enterrow("Clear Peds",	   [&] { GAMEPLAY::CLEAR_AREA_OF_PEDS(playerPos.x, playerPos.y, playerPos.z, 10.0f, TRUE); }),
					enterrow("Clear Ped Props",[&] { PED::CLEAR_ALL_PED_PROPS(selfPed); }),
					enterrow("Clear Objects",  [&] { GAMEPLAY::CLEAR_AREA_OF_OBJECTS(playerPos.x, playerPos.y, playerPos.z, 10.0f, TRUE); }),
					enterrow("Clear Vehicles", [&] {
						Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0 , 0 , 0);
						GAMEPLAY::CLEAR_AREA_OF_VEHICLES(offset.x,offset.y,offset.z,500.00, FALSE, FALSE, FALSE, FALSE, FALSE); 
					}),
					inputrow("Get Hash", 999,   [](char* result) { DrawNotification(fmt::format("Hash of {0} = {1:#X}", result, $(result))); }),
					//introw("Tunable", &currTunable, 0, 10000, 1),
					//enterrow("Get Tunable Int", [] { DrawNotification(std::to_string(GetTunable<int>(currTunable))); }),
					//enterrow("Get Tunable Float", [] { DrawNotification(std::to_string(GetTunable<float>(currTunable))); }),
					//enterrow("Set Tunable To 1", [] { SetTunable(currTunable, 1); }),
					//enterrow("Set Tunable To 0", [] { SetTunable(currTunable, 0); }),
					//enterrow("Set Tunable To 0.1", [] { SetTunable<float>(currTunable, 0.25); }),
				}

			}
		},
		{ "SetTime",
			{"SetTime", "Misc",
				{
					enterrow("Hour forward",		   [] {
							NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(TIME::GET_CLOCK_HOURS() == 23?0:TIME::GET_CLOCK_HOURS() + 1, TIME::GET_CLOCK_MINUTES(), TIME::GET_CLOCK_SECONDS());
					}),
						enterrow("Hour backward",		   [] { NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(TIME::GET_CLOCK_HOURS() ==0 ? 23 : TIME::GET_CLOCK_HOURS() - 1,TIME::GET_CLOCK_MINUTES(),TIME::GET_CLOCK_SECONDS());
					}),
						boolfuncrow("Freeze time", &frezeTime,	[] { 
						fHour = TIME::GET_CLOCK_HOURS();
						fMinute = TIME::GET_CLOCK_MINUTES();
						fsecond = TIME::GET_CLOCK_SECONDS();
						;
					}),
				}
			}
		},
		{ "SetWeather", 
			{ "SetWeather", "Misc",
				{
					dynmenurow("Set weather", "Change Weather", [&] { Menus["Change Weather"] = WeatherMenu(); }),
				}
			}
		},
		{ "ToggleRadio",
			{ "ToggleRadio", "Misc",
				{
					enterrow("Next Track", [] { 
						if (ENTITY::DOES_ENTITY_EXIST( PLAYER::PLAYER_PED_ID() ) ) AUDIO::SKIP_RADIO_FORWARD(); }),
					boolfuncrow("Disable radio", &disableRadio , [] {ToggleRadio(!disableRadio);}),
					boolfuncrow("Enable walk radio", &enableWalkRadio , [] {ToggleWalkRadio(enableWalkRadio);}),
				}
			}
		},
	};

	#pragma endregion

	if (ENTITY::DOES_ENTITY_EXIST(selfPed) == TRUE) // Only draw/update the menu if the player actaully exists.
	{
		#pragma region Initialization

		if (!initialized) // Put stuff that needs to be generated on startup here.
		{			
			WAIT(0); // The menu might flash, but i feel like it should make refreshing (especially when there are more things to initialize) faster

			DrawNotification("Initializing Menu");

			clock_t elapsed;

			elapsed = clock();

			currMenu = Menus["Main"];

			prevSelections.clear();

			std::vector<std::pair<std::string, Menu>> spawnMenus =
			{
				{ "SpawnCompacts",		 { "Compacts",			"VehicleSpawn", { } } },
				{ "SpawnCoupes",		 { "Coupes",			"VehicleSpawn", { } } },
				{ "SpawnSedans",		 { "Sedans",			"VehicleSpawn", { } } },
				{ "SpawnSUVs",			 { "SUVs",				"VehicleSpawn", { } } },
				{ "SpawnMuscle",		 { "Muscle",			"VehicleSpawn", { } } },
				{ "SpawnSportsClassics", { "Sports Classics",	"VehicleSpawn", { } } },
				{ "SpawnSports",		 { "Sports",			"VehicleSpawn", { } } },
				{ "SpawnSuper",			 { "Super",				"VehicleSpawn", { } } },
				{ "SpawnMotorcycle",	 { "Motorcycles",		"VehicleSpawn", { } } },
				{ "SpawnOffroad",		 { "Off-Road",			"VehicleSpawn", { } } },
				{ "SpawnIndustrial",	 { "Industrial",		"VehicleSpawn", { } } },
				{ "SpawnUtility",		 { "Utility",			"VehicleSpawn", { } } },
				{ "SpawnVansPickups",	 { "Vans/Pickups",		"VehicleSpawn", { } } },
				{ "SpawnBicycle",		 { "Bicycles",			"VehicleSpawn", { } } },
				{ "SpawnBoats",			 { "Boats",				"VehicleSpawn", { } } },
				{ "SpawnHelicopter",	 { "Helicopters",		"VehicleSpawn", { } } },
				{ "SpawnAirplane",		 { "Airplanes",			"VehicleSpawn", { } } },
				{ "SpawnService",		 { "Service",			"VehicleSpawn", { } } },
				{ "SpawnEmergency",		 { "Emergency",			"VehicleSpawn", { } } },
				{ "SpawnMilitary",		 { "Military",			"VehicleSpawn", { } } },
				{ "SpawnCommercial",	 { "Commercial",		"VehicleSpawn", { } } },
				{ "SpawnOther",			 { "Other",				"VehicleSpawn", { } } },
			};

			for each (char* vehicleName in VehicleHashes)
			{
				Hash hash = $(vehicleName);
				char* codeName = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash);
				char* displayName = UI::_GET_LABEL_TEXT(codeName);
				int vehicleClass = int(min(VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(hash), spawnMenus.size() - 1));

				std::string name = fmt::format("{0} ({1})", AreStringsEqual(displayName, "NULL") ? codeName : displayName, vehicleName);

				spawnMenus[vehicleClass].second.rows.push_back(enterrow(name, [&, hash] { SpawnPlayerVehicle(selfPed, hash, spawnInVehicle, baseUpgrades, maxUpgrades,preventKick); }));
			}

			sort(spawnMenus.begin(), spawnMenus.end(), MenuPairSort);

			for each(std::pair<std::string, Menu> menu in spawnMenus)
			{
				sort(menu.second.rows.begin(), menu.second.rows.end(), MenuRowSort);
				Menus.insert(menu);
			}
			#pragma region bonelist
			aimbotBones = {
				SKEL_ROOT,
				SKEL_Pelvis,
				SKEL_L_Thigh,
				SKEL_L_Calf,
				SKEL_L_Foot,
				SKEL_L_Toe0,
				SKEL_R_Thigh,
				SKEL_R_Calf ,
				SKEL_R_Foot ,
				SKEL_R_Toe0 ,
				SKEL_Spine0 ,
				SKEL_Spine1 ,
				SKEL_Spine2 ,
				SKEL_Spine3 ,
				SKEL_L_Clavicle,
				SKEL_L_UpperArm,
				SKEL_L_Forearm ,
				SKEL_L_Hand ,
				SKEL_L_Finger00 ,
				SKEL_L_Finger01 ,
				SKEL_L_Finger02 ,
				SKEL_L_Finger10 ,
				SKEL_L_Finger11 ,
				SKEL_L_Finger12 ,
				SKEL_L_Finger20 ,
				SKEL_L_Finger21 ,
				SKEL_L_Finger22 ,
				SKEL_L_Finger30 ,
				SKEL_L_Finger31 ,
				SKEL_L_Finger32 ,
				SKEL_L_Finger40 ,
				SKEL_L_Finger41 ,
				SKEL_L_Finger42 ,
				SKEL_R_Clavicle ,
				SKEL_R_UpperArm ,
				SKEL_R_Forearm ,
				SKEL_R_Hand ,
				SKEL_R_Finger00,
				SKEL_R_Finger01,
				SKEL_R_Finger02,
				SKEL_R_Finger10,
				SKEL_R_Finger11,
				SKEL_R_Finger12,
				SKEL_R_Finger20,
				SKEL_R_Finger21,
				SKEL_R_Finger22,
				SKEL_R_Finger30,
				SKEL_R_Finger31,
				SKEL_R_Finger32,
				SKEL_R_Finger40,
				SKEL_R_Finger41,
				SKEL_R_Finger42,
				SKEL_Neck_1,
				SKEL_Head,
				
			};
			#pragma endregion
			DrawNotification(fmt::format("Finished initializing in {0} miliseconds", clock() - elapsed));

			initialized = true;
		}

		#pragma endregion

		UpdateKeyPresses();
		bool f5JustPressed = IsKeyPressed(INPUT_FRONTEND_SOCIAL_CLUB);
		if (f5JustPressed)
		{
			menuVisible = !menuVisible; // If F5 is just pressed, toggle the menu.
			AUDIO::PLAY_SOUND_FRONTEND(-1, "TOGGLE_ON", SoundSet, 1);
		}
		if (UI::IS_PAUSE_MENU_ACTIVE())
			menuVisible = false;
		if (menuVisible)
		{
			hideCellphone();
			bool enterJustPressed = IsKeyPressed(INPUT_FRONTEND_ACCEPT),
				upJustPressed = IsKeyPressed(INPUT_FRONTEND_UP),
				downJustPressed = IsKeyPressed(INPUT_FRONTEND_DOWN),
				leftJustPressed = IsKeyPressed(INPUT_FRONTEND_LEFT),
				rightJustPressed = IsKeyPressed(INPUT_FRONTEND_RIGHT),
				backJustPressed = IsKeyPressed(INPUT_FRONTEND_CANCEL);
				 
			#pragma region Play Sounds

			if (backJustPressed && spectateCam ) {
				AI::CLEAR_PED_TASKS(selfPed);
				NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(FALSE, PLAYER::GET_PLAYER_PED(currentPlayer));
				spectateCam = false;
			}
			Menus["Online"].rows.clear(); // Clear the current player list
			int hostID = NETWORK::NETWORK_GET_HOST_OF_SCRIPT(SCRIPT::GET_THIS_SCRIPT_NAME(), -1, 0);

			for (int i = 0; i < 32; i++) // Iterate through the possible players, adding them to the list.
			{
				char* playerName = GetPlayerName(i);
				if (!GAMEPLAY::ARE_STRINGS_EQUAL(playerName, "**Invalid**")) // Only add valid players to the list
				{
					Player playerNum = IntToPlayer(i);
					std::string host = "~r~[Likely Host]";
					if (i == hostID)
					{
						Menus["Online"].rows.push_back(enterrow(playerName + host, [=] { currentPlayer = playerNum; Menus["Selected Player"].title = PLAYER::GET_PLAYER_NAME(currentPlayer); currMenu = Menus["Selected Player"];  }));
					}
					else
					{
						Menus["Online"].rows.push_back(enterrow(playerName, [=] { currentPlayer = playerNum; Menus["Selected Player"].title = PLAYER::GET_PLAYER_NAME(currentPlayer); currMenu = Menus["Selected Player"];  }));
					}
				}
			}

			if (enterJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", SoundSet, TRUE);
			if (backJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", SoundSet, TRUE);
			if (upJustPressed || downJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", SoundSet, TRUE);
			if (leftJustPressed || rightJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", SoundSet, TRUE);

			#pragma endregion			

			if (currMenu.rows.size() == 0)
			{
				backJustPressed = true;
			}
			if (backJustPressed)
			{
					prevSelections[currMenu.title] = 0;
					currMenu = Menus[currMenu.previous];
			}

			currentSelectionIndex = prevSelections[currMenu.title];

			// Move the current selection up/down depending on the pressed keys.
			if (upJustPressed)
			{
				currentSelectionIndex -= 1;
			}			
			if (downJustPressed)
			{
				currentSelectionIndex += 1;
			}

			int currSize = int(currMenu.rows.size()); // Current Menu Size

			currentSelectionIndex = modulo(currentSelectionIndex, currSize); // Using custom modulo for negative numbers

			prevSelections[currMenu.title] = currentSelectionIndex;

			int end = min(currSize, max(0, currentSelectionIndex - Middle) + MaxRows);
			int start = max(0, end - MaxRows);
			int length = end - start; // Length = start - end

			float drawnY = MenuY;
			
			#pragma region Draw Menu
			DrawMenuLine(currMenu.title, MenuX, drawnY, MenuWidth, TitleHeight, TitleFont, TitleTextScale, TextMargin, true, Center, TitleTextColor, TitleRectColor);		
			drawnY += TitleHeight;

			for (int i = 0; i < length; i++)
			{
				int current = i + start; // Get the index of the row to be drawn

				MenuRow currRow = currMenu.rows[current];

				if (current == currentSelectionIndex)
					DrawMenuLine(strchr(currRow.name), MenuX, drawnY, MenuWidth, RowHeight, ActiveFont, ActiveTextScale, TextMargin, true, Left, ActiveTextColor, ActiveRectColor);
				else
					DrawMenuLine(strchr(currRow.name), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, true, Left, DefaultTextColor, DefaultRectColor);

				switch (currRow.type)
				{
					case TOGGLE:
						if (*currRow.boolptr)
							DrawMenuLine("On", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OnTextColor, DefaultRectColor);
						else
							DrawMenuLine("Off", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OffTextColor, DefaultRectColor);
						break;
					case TOGGLEF:
						if (*currRow.boolptr)
							DrawMenuLine("On", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OnTextColor, DefaultRectColor);
						else
							DrawMenuLine("Off", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OffTextColor, DefaultRectColor);
						break;
					case INTSLIDER:
						DrawMenuLine(strchr(std::to_string(*currRow.intptr)), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, IntTextColor, DefaultRectColor);
						break;
					case INTSLIDERF:
						DrawMenuLine(strchr(std::to_string(*currRow.intptr)), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, IntTextColor, DefaultRectColor);
						break;
					case SUBMENU:
						DrawMenuLine(">>", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, MenuTextColor, DefaultRectColor);
						break;
					case DYNAMICSUBMENU:
						DrawMenuLine(">>", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, MenuTextColor, DefaultRectColor);
						break;
					case GETINPUT:
						DrawMenuLine("> _", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, DefaultTextColor, DefaultRectColor);
						break;
					default:
						break;
				}

				drawnY += RowHeight;
			}

			DrawLine(MenuX, drawnY, MenuWidth, LineHeight, TitleRectColor);
			drawnY += LineHeight;

			DrawMenuLine(strchr(fmt::format("{0} - {1}/{2}", "ZUK Menu 1.33", currentSelectionIndex + 1, currSize)), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, true, Right, DefaultTextColor, DefaultRectColor);
						
			drawnY += RowHeight;

			#pragma endregion

			#pragma region Update Selection

			MenuRow currRow = currMenu.rows[currentSelectionIndex];

			switch (currRow.type)
			{
				case SUBMENU:
					if (enterJustPressed)
					{
						if (Menus[currRow.nextmenu].rows.size() > 0)
						{
							currMenu = Menus[currRow.nextmenu];
						}
					}
					break;
				case DYNAMICSUBMENU:
					if (enterJustPressed)
					{
						currRow.enterFunction();
						if (Menus[currRow.nextmenu].rows.size() > 0)
						{
							currMenu = Menus[currRow.nextmenu];
						}
					}
					break;
				case ENTER:
					if (enterJustPressed)
					{
						currRow.enterFunction();
					}
					break;
				case TOGGLE:
					if (enterJustPressed || leftJustPressed || rightJustPressed)
					{
						*currRow.boolptr ^= true;
					}
					break;
				case TOGGLEF:
					if (enterJustPressed || leftJustPressed || rightJustPressed)
					{
						*currRow.boolptr ^= true;
						currRow.enterFunction();
					}
					break;
				case INTSLIDER:
					if (leftJustPressed)
					{
						*currRow.intptr -= currRow.increment;
					}
					if (rightJustPressed)
					{
						*currRow.intptr += currRow.increment;
					}
					if (*currRow.intptr > currRow.max)
					{
						*currRow.intptr = currRow.min;
					}
					if (*currRow.intptr < currRow.min)
					{
						*currRow.intptr = currRow.max;
					}
					break;
				case INTSLIDERF:
					if (leftJustPressed)
					{
						*currRow.intptr -= currRow.increment;
					}
					if (rightJustPressed)
					{
						*currRow.intptr += currRow.increment;
					}
					if (*currRow.intptr > currRow.max)
					{
						*currRow.intptr = currRow.min;
					}
					if (*currRow.intptr < currRow.min)
					{
						*currRow.intptr = currRow.max;
					}
					if (enterJustPressed || ((leftJustPressed || rightJustPressed) && currRow.autoEnter))
					{
						currRow.enterFunction();
					}
					break;
				case GETINPUT:
					if (enterJustPressed)
					{
						currRow.enterFunction();
					}
					break;
				default:
					break;
			}
			if (IsControllerConnected()) {
				instructionalSetup();
				addInstructional("Exit Menu", BUTTON_B, 0);
				addInstructional("Open Submenu", BUTTON_A, 1);
				addInstructional("Navigate", BUTTON_DPAD_ALL, 2);
				instructionalClose();
			}
			#pragma endregion
			Menus["Self"].title = PLAYER::GET_PLAYER_NAME(selfPlayer);

		}
#pragma region Update Functions
		
		UpdateEntitySpawns();
		
		if (autoFixOutFit && !PED::IS_PED_DEAD_OR_DYING(selfPed, 0) && !NETWORK::NETWORK_IS_PLAYER_IN_MP_CUTSCENE(selfPlayer)  && !NETWORK::IS_PLAYER_IN_CUTSCENE(selfPlayer) && !CUTSCENE::IS_CUTSCENE_PLAYING()) {
				applyOutfit(selfPed, *currentOutfit,FALSE);
		
		}

		if (playerGodMode) {
			ENTITY::SET_ENTITY_INVINCIBLE(selfPed, playerGodMode);
			ENTITY::SET_ENTITY_PROOFS(selfPed, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode);
		}

		ENTITY::SET_ENTITY_INVINCIBLE(selfVehicle, vehicleGodMode);
		ENTITY::SET_ENTITY_PROOFS(selfVehicle, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode);

		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(selfPed, vehicleSeatBelt); // 0 = can, 1 = cant;

		if (vehicleSeatBelt)
		{
			PED::SET_PED_CONFIG_FLAG(selfPed, 32, FALSE);
		}

		ToggleRadio(!disableRadio);
		if (frezeTime) 
			NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(fHour, fMinute, fsecond);
		
		if (!PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0)) {
			if(fastRun)
				setPlayerFastRun(selfPlayer, fastRun);
			if(fastSwim)
				setPlayerFastSwim(selfPlayer, fastSwim);
			ToggleWalkRadio(enableWalkRadio);
		}
		esp(selfPed, featurePlayerESP, featurePlayerESPBoxes, featurePlayerESPTrace,featurePlayerSkeleton,featurePedSkeleton,&aimbotBones);

		if (infiniteAmmo)
			InfiniteAmmo();

		if (explosiveAmmo)
		{
			if (ENTITY::DOES_ENTITY_EXIST(selfPed))
				GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(selfPlayer);
		}

		if (neverWanted && PLAYER::GET_PLAYER_WANTED_LEVEL(selfPlayer) > 0) {
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(selfPlayer);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(selfPlayer, FALSE);
		}		

		if(playerInvisible)
			setPlayerVisible(PLAYER::PLAYER_PED_ID(), !playerInvisible);

		if(disableCollission)
			ENTITY::SET_ENTITY_COLLISION(selfPed, !disableCollission, TRUE);

		if (flyToggle)
			VehicleFly(selfVehicle, yaw);
		if (vehicleRandomColor && PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0) && vehicleUpdateTime + 400.0 < GetTickCount())
		{
			vehicleUpdateTime = GetTickCount();
			int colorPrimary = 0, colorSecondary = 0;
			VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(selfVehicle);
			VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(selfVehicle);
			VEHICLE::GET_VEHICLE_COLOURS(selfVehicle, &colorPrimary, &colorSecondary);
			VEHICLE::SET_VEHICLE_COLOURS(selfVehicle, rand() % (159 + 1), rand() % (159 + 1));
		}
		if ((vehicleRainbowColor || vehicleRainbowColorPastel) && PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0) && vehicleUpdateTime + 50.0 < GetTickCount()) {
			vehicleUpdateTime = GetTickCount();
			int len = 220;
			double freq = 2 * M_PI / len;
			color_t color = vehicleRainbowColorPastel ?
				makeColorGradient(vehicleColorSineindex++, freq, freq, freq,
					2 * M_PI / 3,  0, 4 * M_PI / 3,
					200, 55, len)
				:makeColorGradient(vehicleColorSineindex++, freq, freq, freq,
					2 * M_PI / 3,  0, 4 * M_PI / 3,
					128, 127, len);
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(selfVehicle, color.red, color.green, color.blue);
			if(syncRainbowcolors)
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(selfVehicle, color.red, color.green, color.blue);
			if (syncRainbowcolorsTyre)
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(selfVehicle, color.red, color.green, color.blue);
			if (syncRainbowcolorsNeon)
				VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(selfVehicle, color.red, color.green, color.blue);

			int closeColor = getClosestStandartColor(&color);
			if (syncRainbowWheels || syncRainbowPerl) {
				int * perlColor = new int(), *wheelColour = new int();
				VEHICLE::GET_VEHICLE_EXTRA_COLOURS(selfVehicle, perlColor, wheelColour);
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(selfVehicle, syncRainbowWheels ? closeColor : *wheelColour, syncRainbowPerl ? closeColor : *perlColor);
			}
			if (syncRainbowInt) {
				int * col = new int();
				int *colType = new int();
				int * p3 = new int();
				VEHICLE::GET_VEHICLE_MOD_COLOR_1(selfVehicle, colType, col, p3);
				VEHICLE::SET_VEHICLE_MOD_COLOR_1(selfVehicle, *colType, closeColor,*p3);
			}
				
			if (syncRainbowDash) {
				int * col = new int();
				int *colType = new int();
				VEHICLE::GET_VEHICLE_MOD_COLOR_2(selfVehicle, colType, col);
				VEHICLE::SET_VEHICLE_MOD_COLOR_2(selfVehicle, *colType, closeColor);
			}
				
		}
		if (vehicleRainbowColor && !PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0))
			vehicleRainbowColor = false;
		if (vehicleRainbowColorPastel && !PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0))
			vehicleRainbowColorPastel = false;

		if (playerAimbot  && aimbotShootTimer + aimbotShootTimerAdvance < GetTickCount()) {
			aimbotShootTimer = GetTickCount();
			aimbotShootTimerAdvance = rand() % (400);
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())) {
				if (PLAYER::GET_NUMBER_OF_PLAYERS() > 1) {
					for (int i = 0;i < 32;i++)
						if ((PLAYER::PLAYER_PED_ID() != PLAYER::GET_PLAYER_PED(i)) && !ENTITY::IS_ENTITY_DEAD((PLAYER::GET_PLAYER_PED(i)))) {
							auto name = PLAYER::GET_PLAYER_NAME(i);
							if (strcmp(name, "**Invalid**")==0)
								continue;
							if (IsPlayerFriend(PLAYER::INT_TO_PLAYERINDEX(i)))
								continue;
							if (!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(selfPed, PLAYER::GET_PLAYER_PED(i)))
								continue;
							Vector3 coords = PED::GET_PED_BONE_COORDS(PLAYER::GET_PLAYER_PED(i), randomBoneAimbot? randomBone(&aimbotBones):SKEL_Head, 0.0f, 0.0f, 0.0f);
							if (legitAimbotSpray)
								sprayBullet(&coords);
							
							if(aimbotTrace){
								Vector3 selfOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0.0f, 0.0f, 0.0f);
								GRAPHICS::DRAW_LINE(selfOffset.x, selfOffset.y, selfOffset.z, coords.x, coords.y, coords.z, 0, 255, 0, 128);
							}
							int count = 1+rand() % 5;
							for (int i = 0;i < count;i++) {
								if (PED::IS_PED_RELOADING(selfPed) ||  PED::IS_PED_FALLING(selfPed) || PED::IS_PED_JUMPING(selfPed) || PED::IS_PED_CLIMBING(selfPed) || !WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(selfPed))
									break;
								PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, true);
							}
								
						}
				}
			}
		}
		if (pedAimbot  && pedAimbotShootTimer + pedAimbotShootTimerAdvance < GetTickCount()) {
			pedAimbotShootTimer = GetTickCount();
			pedAimbotShootTimerAdvance = rand() % 100;
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())) {
				const int numElements = 10;
				const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
				int peds[arrSize];//0 index is the size of the array
				peds[0] = numElements;
				int count = PED::GET_PED_NEARBY_PEDS(selfPed, peds, -1);
				for (int i = 0; i < count; ++i)
				{
					int offsettedID = i * 2 + 2;
					if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
						continue;
					if ((peds[offsettedID]) == selfPed)
						continue;
					if (ENTITY::IS_ENTITY_DEAD((peds[offsettedID])))
						continue;
					if (!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(selfPed, peds[offsettedID]))
						continue;
					Vector3 coords = PED::GET_PED_BONE_COORDS(peds[offsettedID], randomBoneAimbot ? randomBone(&aimbotBones) : SKEL_Head, 0.0f, 0.0f,0.0f);
				
					if (legitAimbotSpray)
						sprayBullet(&coords);
					
					if (aimbotTrace) {
						Vector3 selfOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0.0f, 0.0f, 0.0f);
						GRAPHICS::DRAW_LINE(selfOffset.x, selfOffset.y, selfOffset.z, coords.x, coords.y, coords.z , 0, 255, 0, 128);
					}
					int count = 1 + rand() % 5;
					for (int i = 0;i < count;i++) {
						PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, true);
					}
				}
			}
		}
		
		if (pedTriggerbot ) {
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())) {
				Entity * targetE = new Entity();
				BOOL res =  PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(selfPlayer, targetE);
				if (res && targetE != NULL) {
					for (int i = 0;i < 1;i++) {
						if (!ENTITY::DOES_ENTITY_EXIST(*targetE))
							continue;
						if (PED::IS_PED_A_PLAYER(*targetE))
							continue;
						if (ENTITY::IS_ENTITY_DEAD(*targetE))
							continue;
						Vector3 coords = PED::GET_PED_BONE_COORDS(*targetE, randomBoneAimbot ? randomBone(&aimbotBones) : SKEL_Head, 0.0f, 0.0f, 0.0f);
						if (legitAimbotSpray)
							sprayBullet(&coords);
						if (aimbotTrace) {
							Vector3 selfOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0.0f, 0.0f, 0.0f);
							GRAPHICS::DRAW_LINE(selfOffset.x, selfOffset.y, selfOffset.z, coords.x, coords.y, coords.z, 0, 255, 0, 128);
						}
						PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, true);
					}
					
				}
				
			}
		}

		if (playerTriggerbot && aimbotShootTimer + aimbotShootTimerAdvance < GetTickCount()) {
			aimbotShootTimer = GetTickCount();
			aimbotShootTimerAdvance = rand() % (400);
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())) {
				Entity * targetE = new Entity();
				
				BOOL res = PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(selfPlayer, targetE);
				if (res && targetE != NULL) {
					for (int i = 0;i < 1;i++) {
						if (!ENTITY::DOES_ENTITY_EXIST(*targetE))
							continue;
						if (!PED::IS_PED_A_PLAYER(*targetE))
							continue;
						if (ENTITY::IS_ENTITY_DEAD(*targetE))
							continue;
						if (IsPlayerFriend(PLAYER::INT_TO_PLAYERINDEX(i)))
							continue;
						Vector3 coords = PED::GET_PED_BONE_COORDS(*targetE, randomBoneAimbot ? randomBone(&aimbotBones) : SKEL_Head, 0.0f, 0.0f, 0.0f);
						if (legitAimbotSpray)
							sprayBullet(&coords);
						if (aimbotTrace) {
							Vector3 selfOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0.0f, 0.0f, 0.0f);
							GRAPHICS::DRAW_LINE(selfOffset.x, selfOffset.y, selfOffset.z, coords.x, coords.y, coords.z, 0, 255, 0, 128);
						}
						if (!PED::IS_PED_RELOADING(selfPed) && !PED::IS_PED_FALLING(selfPed) && !PED::IS_PED_JUMPING(selfPed) && !PED::IS_PED_CLIMBING(selfPed) && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(selfPed)) {
							PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, true);
						}
							
					} 
				}

			}
		}
		
		if (objectRemoveGun) {
			Entity * e = new Entity();
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(selfPlayer, e)) {
				RequestControl(*e);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(*e, 1, 1);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(*e, - 1494,  8500,  9000,  0,  0, 1);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(e);
				ENTITY::DELETE_ENTITY(e);
				DrawNotification("Deleted object");
			}
		}
		if (richPeds)
		{
			const int numElements = 10;
			const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
			int peds[arrSize];//0 index is the size of the array
			peds[0] = numElements;
			int count = PED::GET_PED_NEARBY_PEDS(selfPed, peds, -1);
			for (int i = 0; i < count; ++i)
			{
				int offsettedID = i * 2 + 2;
				if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
					continue;
				if ((peds[offsettedID]) == selfPed)
					continue;
				if (ENTITY::IS_ENTITY_DEAD(peds[offsettedID]))
					continue;
				PED::SET_PED_MONEY(peds[offsettedID], 1500 + (rand() % 500));
			}
		}
		if (robPeds)
		{
			const int numElements = 10;
			const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
			int peds[arrSize];//0 index is the size of the array
			peds[0] = numElements;
			int count = PED::GET_PED_NEARBY_PEDS(selfPed, peds, -1);
			
			for (int i = 0; i < count; ++i)
			{
				int offsettedID = i * 2 + 2;
				if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
					continue;
				if ((peds[offsettedID]) == selfPed)
					continue;
				if (ENTITY::IS_ENTITY_DEAD(peds[offsettedID]))
					continue;
				if(PED::GET_PED_MONEY(peds[offsettedID])==0)
					continue;
				for (int i = 0; i < rand() % 10;i++) {
					Vector3 selfOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0.0f, 0.0f, 0.0f);
					Ped p = PED::CLONE_PED(peds[offsettedID],ENTITY::GET_ENTITY_HEADING(peds[offsettedID]),TRUE,TRUE);
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(p, selfOffset.x, selfOffset.y, selfOffset.z + 1, 0, 0, 1);
					PED::APPLY_DAMAGE_TO_PED(p, 500, TRUE);
					PED::EXPLODE_PED_HEAD(p, 0x99AEEB3B);
					WAIT(0);
					ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&p);
					ENTITY::SET_ENTITY_COLLISION(p, false, TRUE);
				}
				Vector3 selfOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selfPed, 0.0f, 0.0f, 0.0f);
				Ped p = peds[offsettedID];
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(p, selfOffset.x, selfOffset.y, selfOffset.z + 1, 0, 0, 1);
				PED::APPLY_DAMAGE_TO_PED(p, 500, TRUE);
				PED::EXPLODE_PED_HEAD(p, 0x99AEEB3B);
				WAIT(0);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&p);
				ENTITY::SET_ENTITY_COLLISION(p, false, TRUE);
			}
		}
		if(disableRagdoll)
			PED::SET_PED_CAN_RAGDOLL(selfPed, !disableRagdoll);

		if (PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0)) {
			VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(selfVehicle, float(torqueMultiplier));
			VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(selfVehicle, float(rpmMultipler));
		}
		
		if (featureVehSlammed && PED::IS_PED_IN_ANY_VEHICLE(selfPed, 0))
		{
			applyForceToEntity(selfVehicle, 0.0f, 0.0f, -0.666f);
			VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(selfVehicle, 1.0f);
			VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(selfVehicle, 100.0f);
		}

#pragma endregion
	}
}
