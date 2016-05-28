#include "../stdafx.h"

char* radioNames[] = { "RADIO_01_CLASS_ROCK", "RADIO_02_POP", "RADIO_03_HIPHOP_NEW", "RADIO_04_PUNK", "RADIO_05_TALK_01", "RADIO_06_COUNTRY", "RADIO_07_DANCE_01", "RADIO_08_MEXICAN", "RADIO_09_HIPHOP_OLD", "RADIO_12_REGGAE", "RADIO_13_JAZZ", "RADIO_14_DANCE_02", "RADIO_15_MOTOWN", "RADIO_20_THELAB", "RADIO_16_SILVERLAKE", "RADIO_17_FUNK", "RADIO_18_90S_ROCK", "RADIO_19_USER", "RADIO_11_TALK_02", "HIDDEN_RADIO_AMBIENT_TV_BRIGHT", "OFF" };

void crashPlayer(Ped ped, Ped playerPed)
{
	const int maxPeds = 10;
	Ped peds[maxPeds];
	Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, FALSE);
	Vector3 targetCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	Ped ClonePed;
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
	int pedz[arrSize];//0 index is the size of the array
	pedz[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(playerPed, pedz, -1);
	for (int i = 0; i < count; ++i)
	{
		int offsettedID = i * 2 + 2;
		if (ENTITY::DOES_ENTITY_EXIST(peds[offsettedID])) {
			ClonePed = peds[offsettedID];
			break;
		}	
	}
	if (ClonePed == 0)
	{
		DrawNotification("Attempt failed");
		return;
	}
	if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerCoords.x, playerCoords.y, playerCoords.z, targetCoords.x, targetCoords.y, targetCoords.z, FALSE) > 250.0f)
	{
		for (int i = 0; i < maxPeds; i++)
		{
			peds[i] = PED::CLONE_PED(ClonePed, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
			WAIT(0);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(peds[i], ped, PED::GET_PED_BONE_INDEX(ped, SKEL_Head), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, FALSE, FALSE, FALSE, FALSE, 2, TRUE);
			WAIT(0);
		}
		if (ENTITY::DOES_ENTITY_EXIST(ped) && ENTITY::DOES_ENTITY_EXIST(peds[maxPeds - 1])) {
			Hash beach_fire = GAMEPLAY::GET_HASH_KEY("prop_beach_fire");
			STREAMING::REQUEST_MODEL(beach_fire);
			while (!STREAMING::HAS_MODEL_LOADED(beach_fire)) WAIT(0);
			Object object = OBJECT::CREATE_OBJECT(beach_fire, 0, 0, 0, true, true, false);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(object, peds[maxPeds-1], PED::GET_PED_BONE_INDEX(peds[maxPeds - 1], SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
		}
		/*
		for (int i = 0; i < maxPeds; i++)
		{
			RequestControl(peds[i]);
			WAIT(0);
			PED::DELETE_PED(&peds[i]);
			WAIT(0);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&peds[i]);
		}
		*/
	}
	return;
}

void ExplodePlayer(Ped ped, int player)
{
	if (PLAYER::IS_PLAYER_PLAYING(player))
	{
		Vector3 playerPosition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);
		FIRE::ADD_EXPLOSION(playerPosition.x, playerPosition.y, playerPosition.z, EXPLOSION_TANKER, 1000.0f, FALSE, FALSE, 0.0f);
	}
	else
	{
		DrawNotification("Player is not playing!");
	}
}

void FramePlayer(Ped selectedPed)
{
	BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(selectedPed);
	if (bPlayerOnlineExists)
	{
		for (int i = 0; i < 32; i++)
		{
			Vector3 playerPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(i), 0.0f, 0.0f, 0.0f);
			FIRE::ADD_OWNED_EXPLOSION(selectedPed, playerPos.x, playerPos.y, playerPos.z, EXPLOSION_TANKER, 1000.0f, TRUE, FALSE, 0.0f);
		}
		DrawNotification("Framed");
	}
}


void RemoveAllPropsFromPlayer(Ped ped)
{
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	Hash modelHashes[] = { 
		0x2E28CA22 /*p_tram_crash_s*/,			0xA50DDDD0 /*prop_bball_arcade_01*/,
		0xEFC4165A /*prop_food_van_01*/,		0x8E8C7A5B /*prop_crashed_heli*/,
		0x456AA864 /*prop_dj_deck_01*/,			0xBE862050 /*prop_portacabin01*/,
		0xB20E5785 /*prop_sculpt_fix*/,			0x58D3B4EA /*prop_micro_01*/,
		0xC42C019A /*prop_ld_ferris_wheel*/,	0x8AF58425 /*prop_lev_des_barge_01*/,
		0x3DC31836 /*prop_tv_flat_01*/,			0xA9BD0D16 /*prop_coke_block_01*/,
		0x1AFA6A0A /*Prop_weed_01*/,			0x4B3D240F /*prop_wheelchair_01*/,
		0x40F52369 /*p_v_43_safe_s*/,			0xF830B63E /*prop_swiss_ball_01*/,
		0xD541462D /*p_ld_soc_ball_01*/,		0x532B1DD1 /*prop_rub_trolley01a*/,
		0x0E3BA450 /*prop_xmas_tree_int*/,		0xFB631122 /*prop_bumper_car_01*/,
		0x5571173D /*prop_beer_neon_01*/,		0x6AD326C2 /*prop_space_rifle*/,
		0x7FFBC1E2 /*prop_dummy_01*/,			0x678FC2DB /*prop_wheelchair_01_s*/,
		0x5869A8F8 /*prop_large_gold*/,			0xE6CB661E /*PROP_CS_DILDO_01*/,
		0x2AE13DFA /*prop_armchair_01*/,		0x29CB0F3C /*prop_armour_pickup*/,
		0x922C2A43 /*prop_big_shit_01*/,		0xFA686C0E /*prop_bin_04a*/,
		0x1F550C17 /*prop_chair_01a*/,			0x5B5C4263 /*prop_chip_fryer*/,
		0x39885BB5 /*prop_chickencoop_a*/,		0x16A39A90 /*prop_dog_cage_01*/,
		0xE3CE09E2 /*prop_dummy_plane*/,		0x927A5723 /*prop_fan_01*/,
		0x34D5D3FD /*prop_golf_bag_01*/,		0xB467C540 /*p_spinning_anus_s*/,
		0x745F3383 /*prop_windmill_01*/,		0x392D62AA /*prop_gold_cont_01*/,
		0x07121AC4 /*prop_xmas_ext*/,			0x0E8032E4 /*prop_weed_pallet*/,
		0xD44295DD /*p_cablecar_s*/,			0x6F9939C7 /*prop_ld_toilet_01*/,
		0x9C762726 /*prop_lev_des_barge_02*/,	0x8973A868 /*prop_air_bigradar*/,
		0xC2BC19CD /*p_cs_mp_jet_01_s*/,		0x26CF04DB /*garbage bin*/,
		0x6DB9599A /*baskedball */ };

	int removed = 0;
	for each (Hash modelHash in modelHashes)
	{		
		for (int i = 0; i < 50000; i++)
		{
			Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(playerPosition.x, playerPosition.y, playerPosition.z, 5.0f, modelHash, TRUE, 0, 0);
			if (ENTITY::DOES_ENTITY_EXIST(obj)) 
			{
				ENTITY::DELETE_ENTITY(&obj);
				removed++;
			}
			else
			{
				break;
			}
		}		
	}

	if (removed > 0)
	{
		DrawNotification("Removed " + std::to_string(removed) + " entities");
	}
}

void RequestControl(Entity entity)
{
	if (!ENTITY::DOES_ENTITY_EXIST(entity))
	{
		DrawNotification("Entity does not exist");
		return;
	}
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
	int tick = 0;
	while (tick <= 50)
	{
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity))
			WAIT(0);
		else
			return;
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		tick++;
	}
	if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity))
		DrawNotification("Request control failed");
}

void requestControlPlayer(Entity index)
{
	Any networkId = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(index);
	int tick = 0;

	while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(networkId) && tick <= 50)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(networkId);
		tick++;
	}
}

bool SpectatePlayer(Ped playerped)
{
	NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, playerped);
	return true;
	Cam  SpectateCam;
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerped, FALSE);
	Vector3 rot = ENTITY::GET_ENTITY_ROTATION(playerped,2);
	rot.x = ENTITY::GET_ENTITY_PITCH(playerped);
	rot.y = ENTITY::GET_ENTITY_ROLL(playerped);
	SpectateCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z+1, rot.x, rot.y, rot.z, 50.0, 0, 2);
	if (!CAM::DOES_CAM_EXIST(SpectateCam)) {
		DrawNotification("Failed to create the cam");
		return false;
	}
	//CAM::ATTACH_CAM_TO_PED_BONE(SpectateCam, playerped, IK_Head, playerPosition.x,
	//	playerPosition.y, playerPosition.z, ENTITY::GET_ENTITY_HEADING(playerped));
	CAM::ATTACH_CAM_TO_ENTITY(SpectateCam, playerped, 0, 0, 5, 1);
	CAM::POINT_CAM_AT_ENTITY(SpectateCam, playerped, 0, 0, 0, 1);
	CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(0);
	CAM::SET_CAM_ACTIVE(SpectateCam, 1);
	CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, SpectateCam, TRUE, TRUE);
	return true;
}

void StartNewScript(char * script)
{
	if (SCRIPT::DOES_SCRIPT_EXIST(script))
	{
		SCRIPT::REQUEST_SCRIPT(script);
		while (SCRIPT::HAS_SCRIPT_LOADED(script) == 0)
			SYSTEM::WAIT(0);
		SYSTEM::START_NEW_SCRIPT(script, 512); // check 1024 - 8192
		DrawNotification("Script requested");
	}
	else DrawNotification("Wrong script");
	SCRIPT::SET_SCRIPT_AS_NO_LONGER_NEEDED(script);
}

void RequestControl2(Entity entity)
{

	if (!ENTITY::DOES_ENTITY_EXIST(entity))
	{
		DrawNotification("Entity does not exist");
		return;
	}
	int netId = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);


	if (NETWORK::NETWORK_DOES_NETWORK_ID_EXIST(netId)) {
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netId);
		int tick = 0;
		while (tick <= 50)
		{
			if (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netId))
				WAIT(0);
			else {
				DrawNotification("got network control");
				return;
			}
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netId);
			tick++;
		}
		DrawNotification("fAILED to get network control");
	}
	else
		DrawNotification("network does not exist");
}

void ClearWantedLevel(Player player)
{
	PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
}

void ReplenishPlayer(Ped playerPed)
{
	if (ENTITY::DOES_ENTITY_EXIST(playerPed) & !ENTITY::IS_ENTITY_DEAD(playerPed))
	{
		PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
		PED::ADD_ARMOUR_TO_PED(playerPed, 342);
		PED::SET_PED_ARMOUR(playerPed, 342);
		PLAYER::SET_PLAYER_MAX_ARMOUR(playerPed, 342);
		PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(playerPed, 333.0f);
		ENTITY::SET_ENTITY_HEALTH(playerPed, 342);
		PLAYER::SET_SPECIAL_ABILITY_MULTIPLIER(333.0f);
		PLAYER::SPECIAL_ABILITY_FILL_METER(playerPed, TRUE);
		PLAYER::SPECIAL_ABILITY_LOCK(playerPed);
	}
}

void SpawnPedMoney(Ped playerPed)
{
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, FALSE);
		Ped spawnedPed = PED::CREATE_RANDOM_PED(playerPosition.x, playerPosition.y, playerPosition.z + 10);
		ENTITY::SET_ENTITY_VELOCITY(spawnedPed, 0, 0, -20);
		ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(spawnedPed, playerPed, FALSE);
		PED::SET_PED_MONEY(spawnedPed, rand() % 2500 + 7499);
		ENTITY::SET_ENTITY_HEALTH(spawnedPed, 0);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedPed);

		DrawNotification("Do not bank any money");
	}
}

char* GetPlayerName(int index)
{
	return PLAYER::GET_PLAYER_NAME(index);
}

Player IntToPlayer(int index)
{
	return PLAYER::INT_TO_PLAYERINDEX(index);
}

void KillPed(Ped ped)
{
	Vector3 position = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	FIRE::ADD_EXPLOSION(position.x, position.y, position.z, EXPLOSION_BLIMP, 100.0f, FALSE, TRUE, 0.0f);
}

void EverythingProof(Ped ped)
{
	RequestControl(ped);
	ENTITY::SET_ENTITY_PROOFS(ped, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
}

void changePedModel(Hash model) {
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);
		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		GiveAllWeapons(PLAYER::PLAYER_PED_ID());
		DrawNotification("Model changed");
	}
}

Menu ModelMenu(/*Hash * originalPedModel,Hash * currentPedModel*/)
{
	#pragma region models
	std::map<std::string, std::string> ClothingCatagories =
	{
		/*
		{ "a_c_chickenhawk", "HAWK" },
		{ "a_c_rat", "RAT" },
		{ "a_f_m_bevhills_01", "BEVHILLS" },
		{ "a_f_m_ktown_01", "KTOWN" },
		{ "a_f_m_tramp_01", "TRAMP" },
		{ "a_f_y_bevhills_03", "BEVHILLS" },
		{ "a_f_y_fitness_01", "FITNESS" },
		{ "a_f_y_indian_01", "INDIAN" },
		{ "a_f_y_topless_01", "TOPLESS" },
		{ "a_m_m_beach_01", "BEACH" },
		{ "a_m_m_genfat_02", "GENFAT" },
		{ "a_m_m_og_boss_01", "OG_BOSS" },
		{ "a_m_m_skidrow_01", "SKIDROW" },
		{ "a_m_m_tramp_01", "TRAMP" },
		{ "a_m_o_soucent_02", "SOUCENT" },
		{ "a_m_y_bevhills_01", "BEVHILLS" },
		{ "a_m_y_eastsa_01", "EASTSA" },
		{ "a_m_y_hiker_01", "HIKER" },
		{ "a_m_y_latino_01", "LATINO" },
		{ "a_m_y_runner_02", "RUNNER" },
		{ "a_m_y_stlat_01", "STLAT" },
		{ "a_m_y_yoga_01", "YOGA" },
		{ "csb_bride", "BRIDE" },
		{ "csb_fos_rep", "FOS_REP" },
		{ "csb_ortega", "ORTEGA" },
		{ "csb_ramp_mex", "MEX" },
		{ "cs_ashley", "ASHLEY" },
		{ "cs_chengsr", "CHENGSR" },
		{ "cs_drfriedlander", "DRFRIEDLANDER" },
		{ "cs_jimmydisanto", "JIMMYDISANTO" },
		{ "cs_manuel", "MANUEL" },
		{ "cs_mrsphillips", "MRSPHILLIPS" },
		{ "cs_paper_p", "PAPER_P" },
		{ "cs_taocheng", "TAOCHENG" },
		{ "g_f_y_families_01", "FAMILIES" },
		{ "g_m_m_chicold_01", "CHICOLD" },
		{ "g_m_y_ballaeast_01", "BALLAEAST" },
		{ "g_m_y_lost_02", "LOST" },
		{ "g_m_y_pologoon_02_p", "POLOGOON_P" },
		{ "hc_hacker", "HC_HACKER" },
		{ "ig_beverly", "BEVERLY" },
		{ "ig_clay", "CLAY" },
		{ "ig_fabien", "FABIEN" },
		{ "ig_jimmydisanto", "JIMMYDISANTO" },
		{ "ig_lifeinvad_02", "LIFEINVAD" },
		{ "ig_mrsphillips", "MRSPHILLIPS" },
		{ "ig_ortega", "ORTEGA" },
		{ "ig_russiandrunk", "RUSSIANDRUNK" },
		{ "ig_taostranslator_p", "TAOSTRANSLATOR_P" },
		{ "mp_f_deadhooker", "DEADHOOKER" },
		{ "mp_m_freemode_01", "FREEMODE" },
		{ "s_f_m_fembarber", "FEMBARBER" },
		{ "s_f_y_hooker_02", "HOOKER" },
		{ "s_f_y_stripper_01", "STRIPPER" },
		{ "s_m_m_ciasec_01", "CIASEC" },
		{ "s_m_m_highsec_01", "HIGHSEC" },
		{ "s_m_m_migrant_01", "MIGRANT" },
		{ "s_m_m_scientist_01", "SCIENTIST" },
		{ "s_m_y_airworker", "AIRWORKER" },
		{ "s_m_y_clown_01", "CLOWN" },
		{ "s_m_y_factory_01", "FACTORY" },
		{ "s_m_y_pilot_01", "PILOT" },
		{ "s_m_y_valet_01", "VALET" },
		{ "u_f_y_bikerchic", "BIKERCHIC" },
		{ "u_m_m_aldinapoli", "ALDINAPOLI" },
		{ "u_m_m_markfost", "MARKFOST" },
		{ "u_m_y_abner", "ABNER" },
		{ "u_m_y_hippie_01", "HIPPIE" },
		{ "player_two", "TREVOR" },
		{ "a_c_cormorant", "CORMORANT" },
		{ "a_c_rhesus", "RHESUS" },
		{ "a_f_m_bevhills_02", "BEVHILLS" },
		{ "a_f_m_ktown_02", "KTOWN" },
		{ "a_f_o_genstreet_01", "GENSTREET" },
		{ "a_f_y_bevhills_04", "BEVHILLS" },
		{ "a_f_y_fitness_02", "FITNESS" },
		{ "a_f_y_juggalo_01", "JUGGALO" },
		{ "a_f_y_tourist_01", "TOURIST" },
		{ "a_m_m_beach_02", "BEACH" },
		{ "a_m_m_golfer_01", "GOLFER" },
		{ "a_m_m_paparazzi_01", "PAPARAZZI" },
		{ "a_m_m_socenlat_01", "SOCENLAT" },
		{ "a_m_m_tranvest_01", "TRANVEST" },
		{ "a_m_o_soucent_03", "SOUCENT" },
		{ "a_m_y_bevhills_02", "BEVHILLS" },
		{ "a_m_y_eastsa_02", "EASTSA" },
		{ "a_m_y_hippy_01", "HIPPY" },
		{ "a_m_y_methhead_01", "METHHEAD" },
		{ "a_m_y_salton_01", "SALTON" },
		{ "a_m_y_stwhi_01", "STWHI" },
		{ "u_m_y_proldriver_01", "PROLDRIVER" },
		{ "csb_burgerdrug", "BURGERDRUG" },
		{ "csb_g", "G" },
		{ "csb_oscar", "OSCAR" },
		{ "csb_reporter", "REPORTER" },
		{ "cs_bankman", "BANKMAN" },
		{ "cs_chrisformage", "CHRISFORMAGE" },
		{ "cs_fabien", "FABIEN" },
		{ "cs_joeminuteman", "JOEMINUTEMAN" },
		{ "cs_marnie", "MARNIE" },
		{ "cs_mrs_thornhill", "MRS_THORNHILL" },
		{ "cs_patricia", "PATRICIA" },
		{ "cs_taostranslator", "TAOSTRANSLATOR" },
		{ "g_f_y_lost_01", "LOST" },
		{ "g_m_m_chicold_01_p", "CHICOLD_P" },
		{ "g_m_y_ballaorig_01", "BALLAORIG" },
		{ "g_m_y_lost_03", "LOST" },
		{ "g_m_y_salvaboss_01", "SALVABOSS" },
		{ "ig_abigail", "ABIGAIL" },
		{ "ig_beverly_p", "BEVERLY_P" },
		{ "ig_claypain", "CLAYPAIN" },
		{ "ig_fbisuit_01", "FBISUIT" },
		{ "ig_joeminuteman", "JOEMINUTEMAN" },
		{ "ig_magenta", "MAGENTA" },
		{ "ig_mrs_thornhill", "MRS_THORNHILL" },
		{ "ig_paper", "PAPER" },
		{ "ig_screen_writer", "SCREEN_WRITER" },
		{ "ig_tenniscoach", "TENNISCOACH" },
		{ "mp_f_freemode_01", "FREEMODE" },
		{ "mp_m_marston_01", "MARSTON" },
		{ "s_f_m_maid_01", "MAID" },
		{ "s_f_y_hooker_03", "HOOKER" },
		{ "s_f_y_stripper_02", "STRIPPER" },
		{ "s_m_m_cntrybar_01", "CNTRYBAR" },
		{ "s_m_m_highsec_02", "HIGHSEC" },
		{ "u_m_y_zombie_01", "ZOMBIE" },
		{ "s_m_m_security_01", "SECURITY" },
		{ "s_m_y_ammucity_01", "AMMUCITY" },
		{ "s_m_y_construct_01", "CONSTRUCT" },
		{ "s_m_y_fireman_01", "FIREMAN" },
		{ "s_m_y_prismuscl_01", "PRISMUSCL" },
		{ "s_m_y_waiter_01", "WAITER" },
		{ "u_f_y_comjane", "COMJANE" },
		{ "u_m_m_bankman", "BANKMAN" },
		{ "u_m_m_partytarget", "PARTYTARGET" },
		{ "u_m_y_antonb", "ANTONB" },
		{ "u_m_y_imporage", "IMPORAGE" },
		{ "a_c_boar", "BOAR" },
		{ "a_c_crow", "CROW" },
		{ "a_c_chop", "CHOP" },
		{ "a_f_m_bodybuild_01", "BODYBUILD" },
		{ "a_f_m_prolhost_01", "PROLHOST" },
		{ "a_f_o_indian_01", "INDIAN" },
		{ "a_f_y_business_01", "BUSINESS" },
		{ "a_f_y_genhot_01", "GENHOT" },
		{ "a_f_y_runner_01", "RUNNER" },
		{ "a_f_y_tourist_02", "TOURIST" },
		{ "a_m_m_bevhills_01", "BEVHILLS" },
		{ "a_m_m_hasjew_01", "HASJEW" },
		{ "a_m_m_polynesian_01", "POLYNESIAN" },
		{ "a_m_m_soucent_01", "SOUCENT" },
		{ "a_m_m_tranvest_02", "TRANVEST" },
		{ "a_m_o_tramp_01", "TRAMP" },
		{ "a_m_y_breakdance_01", "BREAKDANCE" },
		{ "a_m_y_epsilon_01", "EPSILON" },
		{ "a_m_y_hipster_01", "HIPSTER" },
		{ "a_m_y_mexthug_01", "MEXTHUG" },
		{ "a_m_y_skater_01", "SKATER" },
		{ "a_m_y_stwhi_02", "STWHI" },
		
		{ "csb_car3guy1", "CAR3GUY1" },
		{ "csb_groom", "GROOM" },
		{ "csb_porndudes", "PORNDUDES" },
		{ "csb_roccopelosi", "ROCCOPELOSI" },
		{ "cs_barry", "BARRY" },
		{ "cs_clay", "CLAY" },
		{ "cs_fbisuit_01", "FBISUIT" },
		{ "cs_johnnyklebitz", "JOHNNYKLEBITZ" },
		{ "cs_martinmadrazo", "MARTINMADRAZO" },
		{ "cs_natalia", "NATALIA" },
		{ "cs_priest", "PRIEST" },
		{ "cs_tenniscoach", "TENNISCOACH" },
		{ "g_f_y_vagos_01", "VAGOS" },
		{ "g_m_m_chigoon_01", "CHIGOON" },
		{ "g_m_y_ballasout_01", "BALLASOUT" },
		{ "g_m_y_mexgang_01", "MEXGANG" },
		{ "g_m_y_salvagoon_01", "SALVAGOON" },
		{ "ig_amandatownley", "AMANDATOWNLEY" },
		{ "ig_brad", "BRAD" },
		{ "ig_cletus", "CLETUS" },
		{ "ig_floyd", "FLOYD" },
		{ "ig_johnnyklebitz", "JOHNNYKLEBITZ" },
		{ "ig_manuel", "MANUEL" },
		{ "ig_natalia", "NATALIA" },
		{ "ig_patricia", "PATRICIA" },
		{ "ig_siemonyetarian", "SIEMONYETARIAN" },
		{ "ig_terry", "TERRY" },
		{ "mp_f_misty_01", "MISTY" },
		{ "mp_m_niko_01", "NIKO" },
		{ "s_f_m_shop_high", "SHOP_HIGH" },
		{ "s_f_y_migrant_01", "MIGRANT" },
		{ "s_f_y_sweatshop_01", "SWEATSHOP" },
		{ "s_m_m_dockwork_01", "DOCKWORK" },
		{ "s_m_m_janitor", "JANITOR" },
		{ "s_m_m_movprem_01", "MOVPREM" },
		{ "s_m_m_snowcop_01", "SNOWCOP" },
		{ "s_m_y_armymech_01", "ARMYMECH" },
		{ "s_m_y_construct_02", "CONSTRUCT" },
		{ "s_m_y_garbage", "GARBAGE" },
		{ "s_m_y_prisoner_01", "PRISONER" },
		{ "s_m_y_winclean_01", "WINCLEAN" },
		{ "u_f_y_corpse_01", "CORPSE" },
		{ "u_m_m_bikehire_01", "BIKEHIRE" },
		{ "u_m_m_prolsec_01", "PROLSEC" },
		{ "u_m_y_babyd", "BABYD" },
		{ "u_m_y_justin", "JUSTIN" },
		{ "a_c_chimp", "CHIMP" },
		{ "a_c_dolphin", "DOLPHIN" },
		{ "a_c_husky", "HUSKY" },
		{ "a_f_m_business_02", "BUSINESS" },
		{ "a_f_m_salton_01", "SALTON" },
		{ "a_f_o_ktown_01", "KTOWN" },
		{ "a_f_y_business_02", "BUSINESS" },
		{ "a_f_y_golfer_01", "GOLFER" },
		{ "a_f_y_rurmeth_01", "RURMETH" },
		{ "a_f_y_vinewood_01", "VINEWOOD" },
		{ "a_m_m_bevhills_02", "BEVHILLS" },
		{ "a_m_m_hillbilly_01", "HILLBILLY" },
		{ "a_m_m_prolhost_01", "PROLHOST" },
		{ "a_m_m_soucent_02", "SOUCENT" },
		{ "a_m_o_acult_01", "ACULT" },
		{ "a_m_y_acult_01", "ACULT" },
		{ "a_m_y_busicas_01", "BUSICAS" },
		{ "a_m_y_epsilon_02", "EPSILON" },
		{ "a_m_y_hipster_02", "HIPSTER" },
		{ "a_m_y_motox_01", "MOTOX" },
		{ "a_m_y_skater_02", "SKATER" },
		{ "a_m_y_sunbathe_01", "SUNBATHE" },
		{ "u_m_y_sbike", "SBIKE" },
		{ "csb_car3guy2", "CAR3GUY2" },
		{ "csb_grove_str_dlr", "DLR" },
		{ "csb_porndudes_p", "PORNDUDES_P" },
		{ "csb_screen_writer", "SCREEN_WRITER" },
		{ "cs_barry_p", "BARRY_P" },
		{ "cs_dale", "DALE" },
		{ "cs_floyd", "FLOYD" },
		{ "cs_josef", "JOSEF" },
		{ "cs_maryann", "MARYANN" },
		{ "cs_nervousron", "NERVOUSRON" },
		{ "cs_prolsec_02", "PROLSEC" },
		{ "cs_terry", "TERRY" },
		{ "g_m_m_armboss_01", "ARMBOSS" },
		{ "g_m_m_chigoon_01_p", "CHIGOON_P" },
		{ "g_m_y_famca_01", "FAMCA" },
		{ "g_m_y_mexgoon_01", "MEXGOON" },
		{ "g_m_y_salvagoon_02", "SALVAGOON" },
		{ "ig_andreas", "ANDREAS" },
		{ "ig_bride", "BRIDE" },
		{ "ig_dale", "DALE" },
		{ "ig_groom", "GROOM" },
		{ "ig_josef", "JOSEF" },
		{ "ig_marnie", "MARNIE" },
		{ "ig_nervousron", "NERVOUSRON" },
		{ "ig_priest", "PRIEST" },
		{ "ig_solomon", "SOLOMON" },
		{ "ig_tomepsilon", "TOMEPSILON" },
		{ "mp_f_stripperlite", "STRIPPERLITE" },
		{ "mp_m_shopkeep_01", "SHOPKEEP" },
		{ "s_f_m_sweatshop_01", "SWEATSHOP" },
		{ "s_f_y_movprem_01", "MOVPREM" },
		{ "s_m_m_ammucountry", "AMMUCOUNTRY" },
		{ "s_m_m_doctor_01", "DOCTOR" },
		{ "s_m_m_lathandy_01", "LATHANDY" },
		{ "s_m_m_movspace_01", "MOVSPACE" },
		{ "s_m_m_strperf_01", "STRPERF" },
		{ "s_m_y_autopsy_01", "AUTOPSY" },
		{ "s_m_y_cop_01", "COP" },
		{ "s_m_y_grip_01", "GRIP" },
		{ "s_m_y_ranger_01", "RANGER" },
		{ "s_m_y_xmech_01", "XMECH" },
		{ "u_f_y_corpse_02", "CORPSE" },
		{ "u_m_m_fibarchitect", "FIBARCHITECT" },
		{ "u_m_m_promourn_01", "PROMOURN" },
		{ "u_m_y_baygor", "BAYGOR" },
		{ "u_m_y_mani", "MANI" },
		{ "a_c_cow", "COW" },
		{ "a_c_humpback", "HUMPBACK" },
		{ "a_c_mtlion", "MTLION" },
		{ "a_f_m_downtown_01", "DOWNTOWN" },
		{ "a_f_m_skidrow_01", "SKIDROW" },
		{ "a_f_o_salton_01", "SALTON" },
		{ "a_f_y_business_03", "BUSINESS" },
		{ "a_f_y_hiker_01", "HIKER" },
		{ "a_f_y_scdressy_01", "SCDRESSY" },
		{ "a_f_y_vinewood_02", "VINEWOOD" },
		{ "a_m_m_business_01", "BUSINESS" },
		{ "a_m_m_hillbilly_02", "HILLBILLY" },
		{ "a_m_m_rurmeth_01", "RURMETH" },
		{ "a_m_m_soucent_03", "SOUCENT" },
		{ "a_m_o_acult_02", "ACULT" },
		{ "a_m_y_acult_02", "ACULT" },
		{ "a_m_y_business_01", "BUSINESS" },
		{ "a_m_y_gay_01", "GAY" },
		{ "a_m_y_hipster_03", "HIPSTER" },
		{ "a_m_y_motox_02", "MOTOX" },
		{ "a_m_y_soucent_01", "SOUCENT" },
		{ "a_m_y_surfer_01", "SURFER" },
		{ "u_m_y_staggrm_01", "STAGGRM" },
		{ "csb_chef", "CHEF" },
		{ "csb_hao", "HAO" },
		{ "csb_prologuedriver", "PROLOGUEDRIVER" },
		{ "csb_stripper_01", "STRIPPER" },
		{ "cs_beverly", "BEVERLY" },
		{ "cs_davenorton", "DAVENORTON" },
		{ "cs_guadalope", "GUADALOPE" },
		{ "cs_josh", "JOSH" },
		{ "cs_michelle", "MICHELLE" },
		{ "cs_nigel", "NIGEL" },
		{ "cs_russiandrunk", "RUSSIANDRUNK" },
		{ "cs_tom", "TOM" },
		{ "g_m_m_armgoon_01", "ARMGOON" },
		{ "g_m_m_chigoon_02", "CHIGOON" },
		{ "g_m_y_famdnf_01", "FAMDNF" },
		{ "g_m_y_mexgoon_02", "MEXGOON" },
		{ "g_m_y_salvagoon_03", "SALVAGOON" },
		{ "ig_ashley", "ASHLEY" },
		{ "ig_car3guy1", "CAR3GUY1" },
		{ "ig_davenorton", "DAVENORTON" },
		{ "ig_hao", "HAO" },
		{ "ig_josh", "JOSH" },
		{ "ig_maryann", "MARYANN" },
		{ "ig_nigel", "NIGEL" },
		{ "ig_prolsec_02", "PROLSEC" },
		{ "ig_stevehains", "STEVEHAINS" },
		{ "ig_tonya", "TONYA" },
		{ "mp_g_m_pros_01", "PROS" },
		{ "mp_s_m_armoured_01", "ARMOURED" },
		{ "s_f_y_airhostess_01", "AIRHOSTESS" },
		{ "s_f_y_ranger_01", "RANGER" },
		{ "s_m_m_armoured_01", "ARMOURED" },
		{ "s_m_m_fiboffice_01", "FIBOFFICE" },
		{ "s_m_m_lifeinvad_01", "LIFEINVAD" },
		{ "s_m_m_paramedic_01", "PARAMEDIC" },
		{ "s_m_m_strpreach_01", "STRPREACH" },
		{ "s_m_y_barman_01", "BARMAN" },
		{ "s_m_y_dealer_01", "DEALER" },
		{ "s_m_y_hwaycop_01", "HWAYCOP" },
		{ "s_m_y_robber_01", "ROBBER" },
		{ "s_m_y_xmech_02", "XMECH" },
		{ "u_f_y_hotposh_01", "HOTPOSH" },
		{ "u_m_m_filmdirector", "FILMDIRECTOR" },
		{ "u_m_m_rivalpap", "RIVALPAP" },
		{ "u_m_y_burgerdrug_01", "BURGERDRUG" },
		{ "u_m_y_militarybum", "MILITARYBUM" },
		{ "a_c_coyote", "COYOTE" },
		{ "a_c_killerwhale", "WHALE" },
		{ "a_c_retriever", "RETRIEVER" },
		{ "a_f_m_eastsa_01", "EASTSA" },
		{ "a_f_m_soucentmc_01", "SOUCENTMC" },
		{ "a_f_o_soucent_01", "SOUCENT" },
		{ "a_f_y_business_04", "BUSINESS" },
		{ "a_f_y_hippie_01", "HIPPIE" },
		{ "a_f_y_skater_01", "SKATER" },
		{ "a_f_y_vinewood_03", "VINEWOOD" },
		{ "a_m_m_eastsa_01", "EASTSA" },
		{ "a_m_m_indian_01", "INDIAN" },
		{ "a_m_m_salton_01", "SALTON" },
		{ "a_m_m_soucent_04", "SOUCENT" },
		{ "a_m_o_beach_01", "BEACH" },
		{ "a_m_y_beachvesp_01", "BEACHVESP" },
		{ "a_m_y_business_02", "BUSINESS" },
		{ "a_m_y_gay_02", "GAY" },
		{ "a_m_y_indian_01", "INDIAN" },
		{ "a_m_y_musclbeac_01", "MUSCLBEAC" },
		{ "a_m_y_soucent_02", "SOUCENT" },
		{ "a_m_y_vindouche_01", "VINDOUCHE" },
		{ "u_m_y_tattoo_01", "TATTOO" },
		{ "csb_chin_goon", "CHIN_GOON" },
		{ "csb_hugh", "HUGH" },
		{ "csb_prolsec", "PROLSEC" },
		{ "csb_stripper_02", "STRIPPER" },
		{ "cs_beverly_p", "BEVERLY_P" },
		{ "cs_debra", "DEBRA" },
		{ "cs_gurk", "GURK" },
		{ "cs_lamardavis", "LAMARDAVIS" },
		{ "cs_milton", "MILTON" },
		{ "cs_old_man1a", "OLD_MAN1A" },
		{ "cs_siemonyetarian", "SIEMONYETARIAN" },
		{ "cs_tomepsilon", "TOMEPSILON" },
		{ "g_m_m_armlieut_01", "ARMLIEUT" },
		{ "g_m_m_korboss_01", "KORBOSS" },
		{ "g_m_y_famfor_01", "FAMFOR" },
		{ "g_m_y_mexgoon_03", "MEXGOON" },
		{ "g_m_y_salvagoon_03_p", "SALVAGOON_P" },
		{ "ig_ballasog", "BALLASOG" },
		{ "ig_car3guy2", "CAR3GUY2" },
		{ "ig_denise", "DENISE" },
		{ "ig_hunter", "HUNTER" },
		{ "ig_kerrymcintosh", "KERRYMCINTOSH" },
		{ "ig_maude", "MAUDE" },
		{ "ig_old_man1a", "OLD_MAN1A" },
		{ "ig_ramp_gang", "GANG" },
		{ "ig_stretch", "STRETCH" },
		{ "ig_tracydisanto", "TRACYDISANTO" },
		{ "mp_headtargets", "MP_HEADTARGETS" },
		{ "s_f_y_bartender_01", "BARTENDER" },
		{ "s_f_y_scrubs_01", "SCRUBS" },
		{ "s_m_m_armoured_02", "ARMOURED" },
		{ "s_m_m_fiboffice_02", "FIBOFFICE" },
		{ "s_m_m_linecook", "LINECOOK" },
		{ "s_m_m_pilot_01", "PILOT" },
		{ "s_m_m_strvend_01", "STRVEND" },
		{ "s_m_y_baywatch_01", "BAYWATCH" },
		{ "s_m_y_devinsec_01", "DEVINSEC" },
		{ "s_m_y_marine_01", "MARINE" },
		{ "s_m_y_sheriff_01", "SHERIFF" },
		{ "u_f_m_corpse_01", "CORPSE" },
		{ "u_f_y_jewelass_01", "JEWELASS" },
		{ "u_m_m_glenstank_01", "GLENSTANK" },
		{ "u_m_m_spyactor", "SPYACTOR" },
		{ "u_m_y_chip", "CHIP" },
		{ "u_m_y_paparazzi", "PAPARAZZI" },
		{ "a_c_deer", "DEER" },
		{ "a_c_pigeon", "PIGEON" },
		{ "a_c_sharktiger", "SHARKTIGER" },
		{ "a_f_m_eastsa_02", "EASTSA" },
		{ "a_f_m_soucent_01", "SOUCENT" },
		{ "a_f_o_soucent_02", "SOUCENT" },
		{ "a_f_y_eastsa_01", "EASTSA" },
		{ "a_f_y_hipster_01", "HIPSTER" },
		{ "a_f_y_soucent_01", "SOUCENT" },
		{ "a_f_y_vinewood_04", "VINEWOOD" },
		{ "a_m_m_eastsa_02", "EASTSA" },
		{ "a_m_m_ktown_01", "KTOWN" },
		{ "a_m_m_salton_02", "SALTON" },
		{ "a_m_m_stlat_02", "STLAT" },
		{ "a_m_o_genstreet_01", "GENSTREET" },
		{ "a_m_y_beachvesp_02", "BEACHVESP" },
		{ "a_m_y_business_03", "BUSINESS" },
		{ "a_m_y_genstreet_01", "GENSTREET" },
		{ "a_m_y_jetski_01", "JETSKI" },
		{ "a_m_y_musclbeac_02", "MUSCLBEAC" },
		{ "a_m_y_soucent_03", "SOUCENT" },
		{ "a_m_y_vinewood_01", "VINEWOOD" },
		{ "csb_abigail", "ABIGAIL" },
		{ "csb_cletus", "CLETUS" },
		{ "csb_imran", "IMRAN" },
		{ "csb_ramp_gang", "GANG" },
		{ "csb_tonya", "TONYA" },
		{ "cs_brad", "BRAD" },
		{ "cs_denise", "DENISE" },
		{ "cs_hunter", "HUNTER" },
		{ "cs_lazlow", "LAZLOW" },
		{ "cs_molly", "MOLLY" },
		{ "cs_old_man2", "OLD_MAN2" },
		{ "cs_solomon", "SOLOMON" },
		{ "cs_tracydisanto", "TRACYDISANTO" },
		{ "g_m_m_chemwork_01", "CHEMWORK" },
		{ "g_m_m_mexboss_01", "MEXBOSS" },
		{ "g_m_y_korean_01", "KOREAN" },
		{ "g_m_y_mexgoon_03_p", "MEXGOON_P" },
		{ "g_m_y_strpunk_01", "STRPUNK" },
		{ "ig_bankman", "BANKMAN" },
		{ "ig_casey", "CASEY" },
		{ "ig_devin", "DEVIN" },
		{ "ig_janet", "JANET" },
		{ "ig_lamardavis", "LAMARDAVIS" },
		{ "ig_michelle", "MICHELLE" },
		{ "ig_old_man2", "OLD_MAN2" },
		{ "ig_ramp_hic", "HIC" },
		{ "ig_talina", "TALINA" },
		{ "ig_trafficwarden", "TRAFFICWARDEN" },
		{ "mp_m_claude_01", "CLAUDE" },
		{ "s_f_y_baywatch_01", "BAYWATCH" },
		{ "s_f_y_sheriff_01", "SHERIFF" },
		{ "s_m_m_autoshop_01", "AUTOSHOP" },
		{ "s_m_m_gaffer_01", "GAFFER" },
		{ "s_m_m_lsmetro_01", "LSMETRO" },
		{ "s_m_m_pilot_02", "PILOT" },
		{ "s_m_m_trucker_01", "TRUCKER" },
		{ "s_m_y_blackops_01", "BLACKOPS" },
		{ "s_m_y_dockwork_01", "DOCKWORK" },
		{ "s_m_y_marine_02", "MARINE" },
		{ "s_m_y_shop_mask", "SHOP_MASK" },
		{ "u_f_m_miranda", "MIRANDA" },
		{ "u_f_y_mistress", "MISTRESS" },
		{ "u_m_m_griff_01", "GRIFF" },
		{ "u_m_m_willyfist", "WILLYFIST" },
		{ "u_m_y_cyclist_01", "CYCLIST" },
		{ "u_m_y_party_01", "PARTY" },
		{ "a_c_fish", "FISH" },
		{ "a_c_seagull", "SEAGULL" },
		{ "a_c_shepherd", "SHEPHERD" },
		{ "a_f_m_fatbla_01", "FATBLA" },
		{ "a_f_m_soucent_02", "SOUCENT" },
		{ "a_f_y_beach_01", "BEACH" },
		{ "a_f_y_eastsa_02", "EASTSA" },
		{ "a_f_y_hipster_02", "HIPSTER" },
		{ "a_f_y_soucent_02", "SOUCENT" },
		{ "a_f_y_yoga_01", "YOGA" },
		{ "a_m_m_farmer_01", "FARMER" },
		{ "a_m_m_malibu_01", "MALIBU" },
		{ "a_m_m_salton_03", "SALTON" },
		{ "a_m_m_tennis_01", "TENNIS" },
		{ "a_m_o_ktown_01", "KTOWN" },
		{ "a_m_y_beach_01", "BEACH" },
		{ "a_m_y_cyclist_01", "CYCLIST" },
		{ "a_m_y_genstreet_02", "GENSTREET" },
		{ "a_m_y_juggalo_01", "JUGGALO" },
		{ "a_m_y_polynesian_01", "POLYNESIAN" },
		{ "a_m_y_soucent_04", "SOUCENT" },
		{ "a_m_y_vinewood_02", "VINEWOOD" },
		{ "csb_anita", "ANITA" },
		{ "csb_cop", "COP" },
		{ "csb_janitor", "JANITOR" },
		{ "csb_ramp_hic", "HIC" },
		{ "csb_trafficwarden", "TRAFFICWARDEN" },
		{ "cs_bradcadaver", "BRADCADAVER" },
		{ "cs_devin", "DEVIN" },
		{ "cs_janet", "JANET" },
		{ "cs_lestercrest", "LESTERCREST" },
		{ "cs_movpremf_01", "MOVPREMF" },
		{ "cs_omega", "OMEGA" },
		{ "cs_stevehains", "STEVEHAINS" },
		{ "cs_wade", "WADE" },
		{ "g_m_m_chemwork_01_p", "CHEMWORK_P" },
		{ "g_m_m_mexboss_02", "MEXBOSS" },
		{ "g_m_y_korean_02", "KOREAN" },
		{ "g_m_y_pologoon_01", "POLOGOON" },
		{ "g_m_y_strpunk_02", "STRPUNK" },
		{ "ig_barry", "BARRY" },
		{ "ig_chef", "CHEF" },
		{ "ig_dom", "DOM" },
		{ "ig_jay_norris", "JAY_NORRIS" },
		{ "ig_lazlow", "LAZLOW" },
		{ "ig_milton", "MILTON" },
		{ "ig_omega", "OMEGA" },
		{ "ig_ramp_hipster", "HIPSTER" },
		{ "ig_tanisha", "TANISHA" },
		{ "ig_tylerdix", "TYLERDIX" },
		{ "mp_m_exarmy_01", "EXARMY" },
		{ "s_f_y_cop_01", "COP" },
		{ "s_f_y_shop_low", "SHOP_LOW" },
		{ "s_m_m_autoshop_02", "AUTOSHOP" },
		{ "s_m_m_gardener_01", "GARDENER" },
		{ "s_m_m_mariachi_01", "MARIACHI" },
		{ "s_m_m_postal_01", "POSTAL" },
		{ "s_m_m_ups_01", "UPS" },
		{ "s_m_y_blackops_02", "BLACKOPS" },
		{ "s_m_y_doorman_01", "DOORMAN" },
		{ "s_m_y_marine_03", "MARINE" },
		{ "s_m_y_strvend_01", "STRVEND" },
		{ "u_f_m_promourn_01", "PROMOURN" },
		{ "u_f_y_poppymich", "POPPYMICH" },
		{ "u_m_m_jesus_01", "JESUS" },
		{ "u_m_o_finguru_01", "FINGURU" },
		{ "u_m_y_fibmugger_01", "FIBMUGGER" },
		{ "a_c_hen", "HEN" },
		{ "a_c_sharkhammer", "SHARKHAMMER" },
		{ "s_m_m_movalien_01", "ALIEN" },
		{ "a_f_m_fatcult_01", "FATCULT" },
		{ "a_f_m_tourist_01", "TOURIST" },
		{ "a_f_y_bevhills_01", "BEVHILLS" },
		{ "a_f_y_eastsa_03", "EASTSA" },
		{ "a_f_y_hipster_03", "HIPSTER" },
		{ "a_f_y_soucent_03", "SOUCENT" },
		{ "a_m_m_acult_01", "ACULT" },
		{ "a_m_m_fatlatin_01", "FATLATIN" },
		{ "a_m_m_mexcntry_01", "MEXCNTRY" },
		{ "a_m_m_salton_04", "SALTON" },
		{ "a_m_m_tourist_01", "TOURIST" },
		{ "a_m_o_salton_01", "SALTON" },
		{ "a_m_y_beach_02", "BEACH" },
		{ "a_m_y_dhill_01", "DHILL" },
		{ "a_m_y_golfer_01", "GOLFER" },
		{ "a_m_y_ktown_01", "KTOWN" },
		{ "a_m_y_roadcyc_01", "ROADCYC" },
		{ "a_m_y_stbla_01", "STBLA" },
		{ "a_m_y_vinewood_03", "VINEWOOD" },
		{ "csb_anton", "ANTON" },
		{ "csb_customer", "CUSTOMER" },
		{ "csb_maude", "MAUDE" },
		{ "csb_ramp_hipster", "HIPSTER" },
		{ "cs_amandatownley", "AMANDATOWNLEY" },
		{ "cs_carbuyer", "CARBUYER" },
		{ "cs_dom", "DOM" },
		{ "cs_jewelass", "JEWELASS" },
		{ "cs_lifeinvad_01", "LIFEINVAD" },
		{ "cs_movpremmale", "MOVPREMMALE" },
		{ "cs_orleans", "ORLEANS" },
		{ "cs_stretch", "STRETCH" },
		{ "cs_zimbor", "ZIMBOR" },
		{ "g_m_m_chiboss_01", "CHIBOSS" },
		{ "g_m_y_armgoon_02", "ARMGOON" },
		{ "g_m_y_korlieut_01", "KORLIEUT" },
		{ "g_m_y_pologoon_01_p", "POLOGOON_P" },
		{ "hc_driver", "HC_DRIVER" },
		{ "ig_barry_p", "BARRY_P" },
		{ "ig_chengsr", "CHENGSR" },
		{ "ig_dreyfuss", "DREYFUSS" },
		{ "ig_jewelass", "JEWELASS" },
		{ "ig_lestercrest", "LESTERCREST" },
		{ "ig_molly", "MOLLY" },
		{ "ig_oneil", "ONEIL" },
		{ "ig_ramp_mex", "MEX" },
		{ "ig_taocheng", "TAOCHENG" },
		{ "ig_wade", "WADE" },
		{ "mp_m_famdd_01", "FAMDD" },
		{ "s_f_y_factory_01", "FACTORY" },
		{ "s_f_y_shop_mid", "SHOP_MID" },
		{ "s_m_m_bouncer_01", "BOUNCER" },
		{ "s_m_m_gentransport", "GENTRANSPORT" },
		{ "s_m_m_marine_01", "MARINE" },
		{ "s_m_m_postal_02", "POSTAL" },
		{ "s_m_m_ups_02", "UPS" },
		{ "s_m_y_busboy_01", "BUSBOY" },
		{ "s_m_y_dwservice_01", "DWSERVICE" },
		{ "s_m_y_mime", "MIME" },
		{ "s_m_y_swat_01", "SWAT" },
		{ "u_f_o_moviestar", "MOVIESTAR" },
		{ "u_f_y_princess", "PRINCESS" },
		{ "u_m_m_jewelsec_01", "JEWELSEC" },
		{ "u_m_o_taphillbilly", "TAPHILLBILLY" },
		{ "u_m_y_guido_01", "GUIDO" },
		{ "u_m_y_prisoner_01", "PRISONER" },
		*/
		{ "u_m_y_rsranger_01",      "RSRANGER" },
		{ "u_m_y_pogo_01",          "POGO" },
		{ "player_zero",			"MICHAEL" },
		{ "player_one",			    "FRANKLIN" },
		{ "player_two",			    "TREVOR" },
		{ "mp_f_freemode_01",		"MP female" },
		{ "mp_m_freemode_01",		"MP male" },
	};
	#pragma end region

	std::vector<std::pair<std::string, Menu>> menus;

	Menu menu =
	{ "ChangeModel", "Self",
		{
			
		}
	};
	//menu.rows.push_back(enterrow("Reset", [&] {changePedModel(*originalPedModel); *originalPedModel = 0; *currentPedModel = 0; }));
	for each(std::pair<std::string, std::string> clothingCategory in ClothingCatagories)
	{
		menu.rows.push_back(enterrow(clothingCategory.second, [=] {
			changePedModel(GAMEPLAY::GET_HASH_KEY(strchr(fmt::format("{0}", clothingCategory.first))));
			//Hash model = (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_ID()));
			//*originalPedModel = model;
			//DrawNotification(fmt::format("originalPedModel {0}", *originalPedModel));
			//*currentPedModel = model;
			//DrawNotification(fmt::format("currentPedModel {0}", *currentPedModel));
			
		}));
	}
	menu.rows.push_back(inputrow("Custom", 50, [](char* result) { changePedModel(GAMEPLAY::GET_HASH_KEY(result));}));
	return menu;
}

std::vector<std::pair<std::string, Menu>> OverlayMenus(Ped playerPed,bool isFacial = false)
{
	std::vector<std::pair<std::string, Menu>> menus;
	Menu menu =
	{ isFacial ? "Facial Overlays" : "Overlays", "Self",
		{
			enterrow("Default", [=] {
			isFacial ? PED::_CLEAR_PED_FACIAL_DECORATIONS(playerPed) : PED::CLEAR_PED_DECORATIONS(playerPed);
		})
	}
	};


	for each(std::pair<char *, std::vector<OverlayInfo>> collection in mp_overlays)
	{
		char* categoryKey = strchr(fmt::format("Overlays {0}", collection.first));
		menu.rows.push_back(menurow(collection.first, categoryKey));
		Hash currentModel = ENTITY::GET_ENTITY_MODEL(playerPed);
		Menu variationMenu = { strchr(collection.first) , isFacial ? "Facial Overlays" : "Overlays",{} };
		for (int i = 0;i < collection.second.size();++i)
		{
			if (currentModel == $(collection.second[i].getGender()) || collection.second[i].getGender() == "any")
				variationMenu.rows.push_back(enterrow(fmt::format("{0}", collection.second[i].getNameHash()), [=]{
					OverlayInfo overlay = collection.second[i];
					isFacial ?
						PED::_CLEAR_PED_FACIAL_DECORATIONS(playerPed) : 
						PED::CLEAR_PED_DECORATIONS(playerPed);
					isFacial ?
						PED::_SET_PED_FACIAL_DECORATION(playerPed, $(overlay.getCol()), $(overlay.getNameHash())) : 
						PED::_SET_PED_DECORATION(playerPed, $(overlay.getCol()), $(overlay.getNameHash()));
					DrawNotification(fmt::format("Applied {0}", overlay.getNameHash()));
				}
			));
		}
		menus.push_back({ std::string(categoryKey), variationMenu });
	}

	menus.push_back({ isFacial ? "Facial Overlays" : "Overlays", menu });

	return menus;
}

/*
std::vector<std::pair<std::string, Menu>> OverlayMenus(Ped playerPed, Player pl)
{
	std::vector<std::pair<std::string, Menu>> menus;
	Menu menu =
	{ "Overlays", "Self",
		{
			enterrow("Default", [=] {
				PED::CLEAR_PED_DECORATIONS(playerPed);
			})
		}
	};
	
	
	for each(std::pair<char *, std::vector<OverlayInfo>> collection in mp_overlays)
	{
		char* categoryKey = strchr(fmt::format("Overlays {0}", collection.first));
		menu.rows.push_back(menurow(collection.first, categoryKey));
		Hash currentModel = ENTITY::GET_ENTITY_MODEL(playerPed);
		Menu variationMenu = { strchr(collection.first) , "Overlays",{} };
		for each(std::vector<OverlayInfo > overlay in collection.second)
		//for each(std::pair<char *, char * > overlay in collection.second)
		{
			if(currentModel == $(overlay.second))
				variationMenu.rows.push_back(enterrow(fmt::format("{0}", overlay.first), [=] {
					PED::CLEAR_PED_DECORATIONS(playerPed);
					PED::_SET_PED_DECORATION(playerPed, $(collection.first), $(overlay.first));
					DrawNotification(fmt::format("Applied {0}", overlay.second));
				}	
				));
		}
		menus.push_back({ std::string(categoryKey), variationMenu });
	}

	menus.push_back({ "Overlays", menu });

	return menus;
}
*/
std::vector<std::pair<std::string, Menu>> ClothingMenus(Ped playerPed,Player pl, int * headOverOpacity)
{
	std::map<std::string, int> ClothingCatagories = 
	{
		{ "Face",			0  },
		{ "Mask",			1  },
		{ "Haircut",		2  },
		{ "Hands",			3  },
		{ "Trousers",		4  },
		{ "Parachute",		5  },
		{ "Shoes",			6  },
		{ "Eyes",			7  },
		{ "T-Shirt Extras",	8  },
		{ "Body Armour",	9  },
		{ "Decals",			10 },
		{ "T-Shirt/Jacket",	11 },
	};

	std::vector<std::pair<std::string, Menu>> menus;

	Menu menu = 
	{ "Clothing", playerPed == PLAYER::PLAYER_PED_ID()? "Self" : "Selected Player",
		{
			enterrow("Default", [=] {
			if (playerPed != PLAYER::PLAYER_PED_ID()) {
					RequestControl(playerPed);
					PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
					PED::CLEAR_ALL_PED_PROPS(playerPed);
					PED::CLEAR_PED_DECORATIONS(playerPed);
			}
			else
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
				PED::CLEAR_ALL_PED_PROPS(playerPed);
			}),
			enterrow("Random", [=] { 
			if (playerPed != PLAYER::PLAYER_PED_ID())
			{
				RequestControl(playerPed);
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(playerPed, TRUE);
				PED::SET_PED_RANDOM_PROPS(playerPed);
			}
			else
			{
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(playerPed,TRUE);
				PED::SET_PED_RANDOM_PROPS(playerPed);
			}
			}),
		} 
	};

	for each(std::pair<std::string, int> clothingCategory in ClothingCatagories)
	{		
		char* categoryKey = strchr(fmt::format("Clothing {0}", clothingCategory.second));		

		menu.rows.push_back(menurow(clothingCategory.first, categoryKey));

		int variations = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(playerPed, clothingCategory.second);
		
		Menu variationMenu = { strchr(clothingCategory.first) , "Clothing", { } };

		for (int i = 0; i < variations; i++)
		{
			char* variationkey = strchr(fmt::format("{0} {1}", clothingCategory.first, i));			

			int textureVariations = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(playerPed, clothingCategory.second, i);						
			
			if (textureVariations == 1)
			{
				variationMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", i), [=] {
					if (playerPed != PLAYER::PLAYER_PED_ID())
						RequestControl2(playerPed);
					PED::SET_PED_COMPONENT_VARIATION(playerPed, clothingCategory.second, i, 0, 2); }));
			}
			else
			{
				variationMenu.rows.push_back(menurow(fmt::format("Variation #{0} ({1})", i, textureVariations), variationkey));				

				Menu variationTextureMenu = { variationkey , categoryKey,{} };

				for (int j = 0; j < textureVariations; j++)
				{
					variationTextureMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", j), [=] {
						if (playerPed != PLAYER::PLAYER_PED_ID())
							RequestControl2(playerPed);
						PED::SET_PED_COMPONENT_VARIATION(playerPed, clothingCategory.second, i, j, 2); }
					));
				}
				menus.push_back({ std::string(variationkey), variationTextureMenu });
			}
		}

		menus.push_back({ std::string(categoryKey), variationMenu });
	}

		
	std::map<std::string, int> propCatagories =
	{
		{ "Hats",		0 },
		{ "Glasses",	1 },
		{ "Ears",		2 },
	};

	for each(std::pair<std::string, int> clothingCategory in propCatagories)
	{
		char* categoryKey = strchr(fmt::format("Prop {0}", clothingCategory.second));

		menu.rows.push_back(menurow(clothingCategory.first, categoryKey));

		int variations = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(playerPed, clothingCategory.second);

		Menu variationMenu = { strchr(clothingCategory.first) , "Clothing",{} };

		for (int i = 0; i < variations; i++)
		{
			char* variationkey = strchr(fmt::format("{0} {1}", clothingCategory.first, i));

			int textureVariations = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(playerPed, clothingCategory.second, i);

			if (textureVariations == 1)
			{
				variationMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", i), [=] {
					if (playerPed != PLAYER::PLAYER_PED_ID())
						RequestControl2(playerPed);
					PED::SET_PED_PROP_INDEX(playerPed, clothingCategory.second, i, 0, TRUE); }));
			}
			else
			{
				variationMenu.rows.push_back(menurow(fmt::format("Variation #{0} ({1})", i, textureVariations), variationkey));

				Menu variationTextureMenu = { variationkey , categoryKey,{} };

				for (int j = 0; j < textureVariations; j++)
				{
					variationTextureMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", j), [=] {
					if (playerPed != PLAYER::PLAYER_PED_ID())
						RequestControl2(playerPed);
					PED::SET_PED_PROP_INDEX(playerPed, clothingCategory.second, i, j, TRUE); }));
				}
				menus.push_back({ std::string(variationkey), variationTextureMenu });
			}
		}

		menus.push_back({ std::string(categoryKey), variationMenu });
	}

	for each(std::pair<std::string, int> clothingCategory in ClothingCatagories)
	{		
		char* categoryKey = strchr(fmt::format("Clothing {0}", clothingCategory.second));		

		menu.rows.push_back(menurow(clothingCategory.first, categoryKey));

		int variations = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(playerPed, clothingCategory.second);
		
		Menu variationMenu = { strchr(clothingCategory.first) , "Clothing", { } };

		for (int i = 0; i < variations; i++)
		{
			char* variationkey = strchr(fmt::format("{0} {1}", clothingCategory.first, i));			

			int textureVariations = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(playerPed, clothingCategory.second, i);						
			
			if (textureVariations == 1)
			{
				variationMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", i), [=] {
					if (playerPed != PLAYER::PLAYER_PED_ID())
						RequestControl2(playerPed);
					PED::SET_PED_COMPONENT_VARIATION(playerPed, clothingCategory.second, i, 0, 2); }));
			}
			else
			{
				variationMenu.rows.push_back(menurow(fmt::format("Variation #{0} ({1})", i, textureVariations), variationkey));				

				Menu variationTextureMenu = { variationkey , categoryKey,{} };

				for (int j = 0; j < textureVariations; j++)
				{
					variationTextureMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", j), [=] {
						if (playerPed != PLAYER::PLAYER_PED_ID())
							RequestControl2(playerPed);
						PED::SET_PED_COMPONENT_VARIATION(playerPed, clothingCategory.second, i, j, 2); }
					));
				}
				menus.push_back({ std::string(variationkey), variationTextureMenu });
			}
		}

		menus.push_back({ std::string(categoryKey), variationMenu });
	}

	std::map<std::string, int> headOverlays =
	{
		{ "Blemishes",			0 },
		{ "Facial Hair",			1 },
		{ "Eyebrows",		2 },
		{ "Ageing",			3 },
		{ "Makeup",		4 },
		{ "Blush",		5 },
		{ "Complexion",			6 },
		{ "Sun Damage",			7 },
		{ "Lipstick",	8 },
		{ "Moles / Freckles",	9 },
		{ "Chest Hair",			10 },
		{ "Body Blemishes",	11 },
		{ "Add Body Blemishes",	12 },
	};

	menu.rows.push_back(introw("Head overlay opacity", headOverOpacity, 0, 100, 1));
	for each(std::pair<std::string, int> clothingCategory in headOverlays)
	{
		char* categoryKey = strchr(fmt::format("Head overlay {0}", clothingCategory.second));

		menu.rows.push_back(menurow(clothingCategory.first, categoryKey));

		int variations = PED::_GET_NUM_HEAD_OVERLAY_VALUES(clothingCategory.second);

		Menu variationMenu = { strchr(clothingCategory.first) , "Clothing",{} };

		for (int i = 0; i < variations; i++)
		{
			char* variationkey = strchr(fmt::format("{0} {1}", clothingCategory.first, i));
			variationMenu.rows.push_back(enterrow(fmt::format("Variation #{0}", i), [=] { PED::SET_PED_HEAD_OVERLAY(playerPed, clothingCategory.second, i, *headOverOpacity / 100); }));
		}
		variationMenu.rows.push_back(enterrow("Disable", [=] { PED::SET_PED_HEAD_OVERLAY(playerPed, clothingCategory.second, 255, *headOverOpacity /100 ); }));

		menus.push_back({ std::string(categoryKey), variationMenu });
	}
	menus.push_back({ "Clothing", menu });
	return menus;
}

void setPlayerFastSwim(Player player, bool state)
{
	if (state)
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);
	else
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1);
}

void setPlayerFastRun(Player player, bool state)
{
	if (state)
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);
	else
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1);
}

void setPlayerVisible(Ped playerPed, bool state)
{
	ENTITY::SET_ENTITY_VISIBLE(playerPed, state,TRUE);
}

void esp(Ped ped,bool featurePlayerESP,bool featurePlayerESPBoxes,bool featurePlayerESPTrace, bool featurePlayerSkeleton, bool featurePedSkeleton, std::vector<int> *bones)
{
	// player esp
	if (featurePlayerESP)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			Vehicle currentOnlineVeh = PED::GET_VEHICLE_PED_IS_USING(playerOnline);
			Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
			if (playerOnline == ped) continue;
			char* name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(index));
			int health = ENTITY::GET_ENTITY_HEALTH(playerOnline);
			float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerCoords.x, playerCoords.y, playerCoords.z, playerOnlineCoords.x, playerOnlineCoords.y, playerOnlineCoords.z, TRUE);
			int armor = PED::GET_PED_ARMOUR(playerOnline);
			char text[250];
			if (distance < 5000.0f && bPlayerOnlineExists)
			{
				float xa;
				float ya;
				BOOL screenCoords = GRAPHICS::_WORLD3D_TO_SCREEN2D(playerOnlineCoords.x, playerOnlineCoords.y, playerOnlineCoords.z, &xa, &ya);
				if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 17) && bPlayerOnlineExists)
				{
					sprintf_s(text, "%s ~y~%.02f\n~g~%d ~b~%d", name, distance, health, armor);
					UI::SET_TEXT_FONT(0);
					UI::SET_TEXT_OUTLINE();
					UI::SET_TEXT_SCALE(0.2f, 0.2f);
					UI::SET_TEXT_COLOUR(0, 255, 0, 255);
					UI::SET_TEXT_WRAP(0.0f, 1.0f);
					UI::SET_TEXT_CENTRE(TRUE);
					UI::_SET_TEXT_ENTRY("STRING");
					UI::_ADD_TEXT_COMPONENT_STRING(text);
				}
				else
				{
					sprintf_s(text, "%s\n ~y~%.02f", name, distance);
					UI::SET_TEXT_FONT(0);
					UI::SET_TEXT_OUTLINE();
					UI::SET_TEXT_SCALE(0.12f, 0.12f);
					UI::SET_TEXT_COLOUR(255, 0, 0, 75);
					UI::SET_TEXT_WRAP(0.0f, 1.0f);
					UI::SET_TEXT_CENTRE(TRUE);
					UI::_SET_TEXT_ENTRY("STRING");
					UI::_ADD_TEXT_COMPONENT_STRING(text);
				}
				UI::_DRAW_TEXT(xa, ya);
			}
		}
	}

	// player esp boxes
	if (featurePlayerESPBoxes)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			if (playerOnline == ped) continue;
			if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 17) && bPlayerOnlineExists)
			{
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
			}
			else
			{
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
			}
		}
	}

	if (featurePlayerESPTrace)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			if (playerOnline == ped)
				continue;
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			Vector3 pedCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f);
			Vector3 selfCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f);
			if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 17) && bPlayerOnlineExists)
			{
				GRAPHICS::DRAW_LINE(pedCoords.x, pedCoords.y, pedCoords.z, selfCoords.x, selfCoords.y, selfCoords.z, 0, 255, 0, 255);
			}
			else
			{
				GRAPHICS::DRAW_LINE(pedCoords.x, pedCoords.y, pedCoords.z, selfCoords.x, selfCoords.y, selfCoords.z, 255, 0, 0, 75);
			}
		}
	}
	if (featurePlayerSkeleton) {
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			if (playerOnline == ped)
				continue;
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			if (!bPlayerOnlineExists)
				continue;
			std::vector<Vector3> skeleton;
			for each(int bone in *bones) {
				skeleton.push_back(PED::GET_PED_BONE_COORDS(playerOnline, bone, 0.0f, 0.0f, 0.0f));
			}
			Vector3 selfCoors = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f);
			BOOL visible = ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, playerOnline);
			for (int i=0; i < skeleton.size()-1;i++) {
				Vector3 bonePos = skeleton[i];
				Vector3 nexBonePos = skeleton[i+1];
				if(visible)
					GRAPHICS::DRAW_LINE(bonePos.x, bonePos.y, bonePos.z, nexBonePos.x, nexBonePos.y, nexBonePos.z, 0, 255, 0, 255);
				else
					GRAPHICS::DRAW_LINE(bonePos.x, bonePos.y, bonePos.z, nexBonePos.x, nexBonePos.y, nexBonePos.z, 255, 0, 0, 75);
			}
		}
	}
	if (featurePedSkeleton) {
		const int numElements = 10;
		const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
		int peds[arrSize];//0 index is the size of the array
		peds[0] = numElements;
		int count = PED::GET_PED_NEARBY_PEDS(ped, peds, -1);
		for (int i = 0; i < count; ++i)
		{
			int offsettedID = i * 2 + 2;
			if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
				continue;
			if ((peds[offsettedID]) == ped)
				continue;
			if (ENTITY::IS_ENTITY_DEAD(peds[offsettedID]))
				continue;
			BOOL visible = ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, peds[offsettedID]);
			std::vector<Vector3> skeleton;
			for each(int bone in *bones) {
				skeleton.push_back(PED::GET_PED_BONE_COORDS(peds[offsettedID], bone, 0.0f, 0.0f, 0.0f));
			}
			Vector3 selfCoors = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f);
			for (int i = 0; i < skeleton.size() - 1;i++) {
				Vector3 bonePos = skeleton[i];
				Vector3 nexBonePos = skeleton[i + 1];
				if (visible)
					GRAPHICS::DRAW_LINE(bonePos.x, bonePos.y, bonePos.z, nexBonePos.x, nexBonePos.y, nexBonePos.z, 0, 255, 0, 255);
				else
					GRAPHICS::DRAW_LINE(bonePos.x, bonePos.y, bonePos.z, nexBonePos.x, nexBonePos.y, nexBonePos.z, 255, 0, 0, 75);
			}
		}
	}
}

void teleport(bool marker, vector3_t tpCoords)
{
	// get entity to teleport
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	// get coords
	Vector3 coords;
	bool success = false;
	if (marker) // marker
	{
		bool blipFound = false;
		// search for marker blip
		
		int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
		for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
		{
			if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
			{
				coords = UI::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				break;
			}
		}
		if (blipFound)
		{
			// load needed map region and check height levels for ground existence
			bool groundFound = false;
			static float groundCheckHeight[] = {
				100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
				450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
			};
			for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
				WAIT(100);
				if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z,NULL))
				{
					groundFound = true;
					coords.z += 3.0;
					break;
				}
			}
			// if ground not found then set Z in air and give player a parachute
			if (!groundFound)
			{
				coords.z = 1000.0;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
			}
			success = true;
		}
		else
		{
			DrawNotification("Map marker isn't set");
		}

	}
	else // predefined coords
	{
		coords.x = tpCoords.x;
		coords.y = tpCoords.y;
		coords.z = tpCoords.z;
		success = true;
	}
	// set player pos
	if (success)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
		WAIT(0);
		DrawNotification("Teleported");
	}
}

Menu TpLocationsMenu()
{
	Menu savedMenu =
	{ "Locations", "Teleport",
	{

	}
	};

	for each (std::pair<char*, vector3_t> loc in locations)
	{
		savedMenu.rows.push_back(enterrow(fmt::format("{0}", loc.first), [=] { teleport(false, loc.second); }));
	}

	return savedMenu;
}

bool IsPlayerFriend(Player player)
{

	BOOL BplayerFriend = false;
	bool bplayerFriend = false;
	int handle[76];
	NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
	if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
		BplayerFriend = NETWORK::NETWORK_IS_FRIEND(&handle[0]);
	return BplayerFriend ==1? true: false;
}
void check_player_model(Hash * originalPedModel)
{
	//DrawNotification(fmt::format("originalPedModel {0}", *originalPedModel));
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) {
		DrawNotification(fmt::format("entity does not exist"));
		return;
	}
	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		if (model != GAMEPLAY::GET_HASH_KEY("player_zero") &&
			model != GAMEPLAY::GET_HASH_KEY("player_one") &&
			model != GAMEPLAY::GET_HASH_KEY("player_two") &&
			model != GAMEPLAY::GET_HASH_KEY("mp_m_freemode_01") && 
			model != GAMEPLAY::GET_HASH_KEY("mp_f_freemode_01"))
		{
			
			DrawNotification("Turning to normal");
			WAIT(1000);

			if (!STREAMING::IS_MODEL_VALID(*originalPedModel))
				if (NETWORK::NETWORK_IS_SESSION_ACTIVE())
					model = GAMEPLAY::GET_HASH_KEY("mp_m_freemode_01");
				else
					model = GAMEPLAY::GET_HASH_KEY("player_zero");
			else
				model = *originalPedModel;
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model))
				WAIT(0);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

			// wait until player is ressurected
			while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
				WAIT(0);
			//*originalPedModel = 0;
		}
}
void applyOutfit(Ped selfPed, SavedOutfit savedOutfit, bool notification) {
	if (!ENTITY::DOES_ENTITY_EXIST(selfPed)) {
		if(notification)
			DrawNotification(fmt::format("Entity does not exist"));
		return;
	}
	Hash model = ENTITY::GET_ENTITY_MODEL(selfPed);
	if (savedOutfit.pedModel != model)
	{
		//DrawNotification(fmt::format("outfit model {0}", savedOutfit.pedModel));
		//DrawNotification(fmt::format("ped {0}", model));
		if(notification)
			DrawNotification("Wrong outfit model");
		return;
	}
	PED::CLEAR_ALL_PED_PROPS(selfPed);
	for each (std::pair<int,std::pair<int,int>> comp in savedOutfit.comps)
	{
		PED::SET_PED_COMPONENT_VARIATION(selfPed, comp.first, comp.second.first, comp.second.second, 2);
	}
	for each (std::pair<int, std::pair<int, int>> prop in savedOutfit.props)
	{
		PED::SET_PED_PROP_INDEX(selfPed, prop.first, prop.second.first, prop.second.second,TRUE);
	}
	/*
	for each (std::pair<int, std::pair<int, float>> hOver in savedOutfit.headOver)
	{
		PED::SET_PED_HEAD_OVERLAY(selfPed, hOver.first, hOver.second.first, hOver.second.second);
	}
	*/
	if (notification)
		DrawNotification(fmt::format("Outfit {0} applied",savedOutfit.name));
}

SavedOutfit SaveOutfit(char* name, Ped selfPed)
{
	/*
	char* name;
	Hash pedModel;
	std::map<int,std::pair<int, int>> comps;
	std::map<int,std::pair<int, int>> props;
	std::map<int,std::pair<int, float>> headOver;
	*/

	std::map<int, std::pair<int, int>> comps,props;
	//std::map<int, std::pair<int, float>> headOver;

	for (int i = 0; i <= 11; i++)
	{
		int draw = PED::GET_PED_DRAWABLE_VARIATION(selfPed, i);
		int tex = PED::GET_PED_TEXTURE_VARIATION(selfPed, i);
		comps.insert({ i, { draw, tex} });
	}
	for (int i = 0; i <= 2; i++)
	{
		int draw = PED::GET_PED_PROP_INDEX(selfPed, i);
		int tex = PED::GET_PED_PROP_TEXTURE_INDEX(selfPed, i);
		props.insert({ i,{ draw, tex } });
	}
	/*
	for (int i = 0; i <= 12; i++)
	{
		int overVal = PED::_GET_PED_HEAD_OVERLAY_VALUE(selfPed, i);
		float oapcity = 0.0f;
		headOver.insert({ i,{ overVal, oapcity } });
	}
	*/
	DrawNotification(fmt::format("Outfit {0} saved", name));
	DrawNotification(fmt::format("Ped model {0}", ENTITY::GET_ENTITY_MODEL(selfPed)));
	return
	{
		name,
		ENTITY::GET_ENTITY_MODEL(selfPed),
		comps,
		props,
	};
}

Menu SavedOutfitMenu(std::vector<SavedOutfit> savedVehicles, Ped selfPed, SavedOutfit * currentOutfit)
{

	Menu savedMenu;
	if(currentOutfit != NULL)
		savedMenu  =  
	{ "Saved Outfits", "Self",
	{

	}
	};
	else
		savedMenu =
	{ "Saved Outfits", "Selected Player",
	{

	}
	};
	if (currentOutfit != NULL)
		savedMenu.rows.push_back(enterrow("Reset", [=] { currentOutfit->pedModel = NULL; }));
	for each (SavedOutfit savedVehicle in savedVehicles)
	{
		if(ENTITY::GET_ENTITY_MODEL(selfPed) == savedVehicle.pedModel)
			savedMenu.rows.push_back(enterrow(fmt::format("{0}", savedVehicle.name), [=] { 
			if (currentOutfit != NULL) {
				currentOutfit->name = savedVehicle.name;
				currentOutfit->pedModel = savedVehicle.pedModel;
				currentOutfit->props.clear();
				for each(std::pair<int, std::pair<int, int>> p in savedVehicle.props)
					currentOutfit->props[p.first] = p.second;
				currentOutfit->comps.clear();
				for each(std::pair<int, std::pair<int, int>> p in savedVehicle.comps)
					currentOutfit->comps[p.first] = p.second;
			}
			//DrawNotification(fmt::format("currentOutfit->pedModel {0}", currentOutfit->pedModel));
			if (currentOutfit == NULL)
				RequestControl(selfPed);
			applyOutfit(selfPed,  savedVehicle); }));
	}

	return savedMenu;
}

void SaveOutfitsToDisk(std::vector<SavedOutfit> savedVehicles)
{
	std::ofstream myfile;
	myfile.open("outfits_zuk.bin", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!myfile.is_open()) {
		DrawNotification("Could not open file");
		return;
	}
	size_t size = savedVehicles.size();
	myfile.write((char*)&size, sizeof(size_t));
	for each(SavedOutfit veh in savedVehicles) {
		size = strlen(veh.name);
		myfile.write((char*)&size, sizeof(size_t));
		myfile.write(veh.name, size);
		myfile.write((char*)&veh.pedModel, sizeof(Hash));
		size = veh.comps.size();
		myfile.write((char*)&size, sizeof(size_t));
		for each (std::pair<int, std::pair<int, int>> p in veh.comps) {
			myfile.write((char*)&p.first, sizeof(int));
			myfile.write((char*)&p.second.first, sizeof(int));
			myfile.write((char*)&p.second.second, sizeof(int));
		}
		size = veh.props.size();
		myfile.write((char*)&size, sizeof(size_t));
		for each (std::pair<int, std::pair<int, int>> p in veh.props) {
			myfile.write((char*)&p.first, sizeof(int));
			myfile.write((char*)&p.second.first, sizeof(int));
			myfile.write((char*)&p.second.second, sizeof(int));
		}
	}
	myfile.close();
	DrawNotification(fmt::format("Saved {0} outfits", savedVehicles.size()));
}

std::vector<SavedOutfit> LoadOutfitsFromDisk()
{
	std::vector<SavedOutfit> savedVehicles;
	std::ifstream myfile;
	myfile.open("outfits_zuk.bin", std::ios::in | std::ios::binary);
	if (!myfile.is_open()) {
		DrawNotification("Could not open file");
		return savedVehicles;
	}
	bool read_error = false;
	size_t size = 0;
	myfile.read((char*)&size, sizeof(size_t));
	for (size_t i = 0; i < size;i++) {
		SavedOutfit veh;
		size_t c_size = 0;
		myfile.read((char*)&c_size, sizeof(size_t));
		veh.name = new char[c_size + 1];
		myfile.read(veh.name, c_size);
		veh.name[c_size] = '\0';

		myfile.read((char*)&veh.pedModel, sizeof(Hash));
		
		myfile.read((char*)&c_size, sizeof(size_t));
		for (size_t c = 0; c < c_size;c++) {
			int z = 0;
			myfile.read((char*)&z, sizeof(int));
			std::pair<int, int> p;
			myfile.read((char*)&p.first, sizeof(int));
			myfile.read((char*)&p.second, sizeof(int));
			veh.comps[z] = p;
		}
		myfile.read((char*)&c_size, sizeof(size_t));
		for (size_t c = 0; c < c_size;c++) {
			int z = 0;
			myfile.read((char*)&z, sizeof(int));
			std::pair<int, int> p;
			myfile.read((char*)&p.first, sizeof(int));
			myfile.read((char*)&p.second, sizeof(int));
			veh.props[z] = p;
		}
		savedVehicles.push_back(veh);
		//DrawNotification(fmt::format("Vehicle {0} loaded", veh.name));
	}
	myfile.close();
	DrawNotification(fmt::format("Loaded {0} outfits", savedVehicles.size()));
	return savedVehicles;
}

void wantedLevelUp(Player player, Ped playerPed)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) < 5)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) + 1, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		DrawNotification("Wanted up");
	}
}

void wantedLevelDown(Player player, Ped playerPed)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) - 1, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		DrawNotification("Wanted down");
	}
}

void TeleportToMissionObjective(Ped ped)
{
	int blipInfo = UI::GET_FIRST_BLIP_INFO_ID(BLIPCOLOR_MISSION);
	Vector3 BlipCoords;
	if (UI::DOES_BLIP_EXIST(blipInfo))
	{
		BlipCoords = UI::GET_BLIP_COORDS(blipInfo);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(ped, FALSE), BlipCoords.x, BlipCoords.y, BlipCoords.z +2, FALSE, FALSE, FALSE);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, BlipCoords.x, BlipCoords.y, BlipCoords.z +2, FALSE, FALSE, TRUE);
		}
		DrawNotification("Teleported to Mission blip");
	}
	
}

void TeleportToRaceFlag(Ped ped)
{
	int blipInfo = UI::GET_FIRST_BLIP_INFO_ID(BLIP_RACEFLAG);
	Vector3 BlipCoords;
	if (UI::DOES_BLIP_EXIST(blipInfo))
	{
		BlipCoords = UI::GET_BLIP_COORDS(blipInfo);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(ped, FALSE), BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, FALSE);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, TRUE);
		}
		DrawNotification("Teleported to RaceFlag");
	}
}

bool TeleportToCircle(Ped ped)
{
	bool BlipFound = false;
	Vector3 BlipCoords;
	for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(BLIP_CIRCLE); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(BLIP_CIRCLE))
	{
		if (UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_MISSION || UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_YELLOWMISSION || UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_YELLOWMISSION2 || UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_GREEN)
		{
			BlipCoords = UI::GET_BLIP_COORDS(i);
			BlipFound = true;
			break;
		}
	}
	if (BlipFound) {
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(ped, FALSE), BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, FALSE);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, TRUE);
		}
		DrawNotification("Teleported to Circle");
		return true;
	}
	return false;
	int blipInfo = UI::GET_FIRST_BLIP_INFO_ID(BLIP_CIRCLE);
	if (UI::DOES_BLIP_EXIST(blipInfo))
	{
		Vector3 BlipCoords = UI::GET_BLIP_COORDS(blipInfo);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(ped, FALSE), BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, FALSE);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, TRUE);
		}
	}
	
}

void TeleportToBlip(Ped ped,int blipType)
{
	int blipInfo = UI::GET_FIRST_BLIP_INFO_ID(blipType);
	Vector3 BlipCoords;
	if (UI::DOES_BLIP_EXIST(blipInfo))
	{
		BlipCoords = UI::GET_BLIP_COORDS(blipInfo);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(ped, FALSE), BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, FALSE);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, TRUE);
		}
		DrawNotification("Teleported to blip");
	}
}

void TeleportToYelMission(Ped ped)
{
	int blipInfo = UI::GET_FIRST_BLIP_INFO_ID(BLIPCOLOR_YELLOWMISSION);
	Vector3 BlipCoords;
	if (UI::DOES_BLIP_EXIST(blipInfo))
	{
		BlipCoords = UI::GET_BLIP_COORDS(blipInfo);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(ped, FALSE), BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, FALSE);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, BlipCoords.x, BlipCoords.y, BlipCoords.z, FALSE, FALSE, TRUE);
		}
		DrawNotification("Teleported to Yellow Mission blip");
	}
}

void MissionObjective(Ped ped)
{
	if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BLIPCOLOR_MISSION)))
	{
		TeleportToMissionObjective(ped);
	}
	else
	{
		if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BLIPCOLOR_YELLOWMISSION)))
		{
			TeleportToYelMission(ped);
		}
		else
		{
			if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BLIP_CIRCLE)) && TeleportToCircle(ped))
			{
				;
			}
			else
			{
				if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BLIP_RACEFLAG)))
				{
					TeleportToRaceFlag(ped);
				}
				else
				{
					if (UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(BlipSpriteRadar)))
					{
						TeleportToBlip(ped, BlipSpriteRadar);
					}
					else
					{
						DrawNotification("No Mission Objective");
					}
				}
			}
		}
	}
}


bool driving = false;

void Chauffeur(Ped playerPed)
{
	int blipInfo = UI::GET_FIRST_BLIP_INFO_ID(8);
	Vehicle car = PED::GET_VEHICLE_PED_IS_IN(playerPed, FALSE);
	if (!driving)
	{
		if (UI::DOES_BLIP_EXIST(blipInfo))
		{
			STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("SULTANRS"));
			while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("SULTANRS"))) WAIT(0);
			car = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("SULTANRS"), ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0).x, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0).y, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0).z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
			RequestControl(car);
			VEHICLE::SET_VEHICLE_MOD_KIT(car, 0);

			for (int i = 0; i < 50; i++)
			{
				SetVehicleMod(car, i, VEHICLE::GET_NUM_VEHICLE_MODS(car, i) - 1);
			}
			if (car != 0)
			{
				Ped driver = PED::CREATE_RANDOM_PED_AS_DRIVER(car, FALSE);
				ENTITY::SET_ENTITY_VISIBLE(driver, FALSE,TRUE);
				PED::SET_PED_INTO_VEHICLE(driver, car, -1);
				PED::SET_PED_INTO_VEHICLE(playerPed, car, 0);
				AI::TASK_VEHICLE_DRIVE_TO_COORD(driver, car, UI::GET_BLIP_COORDS(blipInfo).x, UI::GET_BLIP_COORDS(blipInfo).y, UI::GET_BLIP_COORDS(blipInfo).z, 100.0, 1, ENTITY::GET_ENTITY_MODEL(car), 6, 6.0f, -1.0f);
				driving = true;
			}
		}
	}
	else if (driving)
	{
		Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(car, -1);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
		WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, 0xFBAB5776);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(driver);
		WEAPON::REMOVE_WEAPON_FROM_PED(driver, 0xFBAB5776);
		ENTITY::SET_ENTITY_HEALTH(driver, 0);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&driver);
		VEHICLE::DELETE_VEHICLE(&car);
		PED::DELETE_PED(&driver);
		driving = false;
	}
	if (!UI::DOES_BLIP_EXIST(blipInfo))
	{
		DrawNotification("Waypoint isn't set");
	}
}

void toNearestCar(Ped ped)
{
	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
	int VehicleHandle = VEHICLE::GET_CLOSEST_VEHICLE(Coords.x, Coords.y, Coords.z, 1000.0, 0, 71);
	Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(VehicleHandle, -1);
	AI::CLEAR_PED_TASKS_IMMEDIATELY(driver);
	WEAPON::REMOVE_WEAPON_FROM_PED(driver, 0xFBAB5776);
	PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), VehicleHandle, -1);
	VEHICLE::SET_VEHICLE_ENGINE_ON(VehicleHandle, TRUE, TRUE,TRUE);
}

int GET_FREE_SEAT(Vehicle VehicleHandle)
{
	int numSeats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(VehicleHandle);
	int SeatIndex = -1;  //Driver seat = -1
	while (SeatIndex < numSeats)
	{
		if (VEHICLE::IS_VEHICLE_SEAT_FREE(VehicleHandle, SeatIndex))
			return SeatIndex;
		SeatIndex++;
	}
	return -2;
}

int SelectedClient;
void TeleportToClient(Ped Client, Ped player)
{

	if (PLAYER::IS_PLAYER_PLAYING(player))
	{
		Vector3 Coords = ENTITY::GET_ENTITY_COORDS(Client, 1);
		if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID()))
			ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), Coords.x, Coords.y, Coords.z + 10, 1, 0, 0, 1);
		else
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(player, TRUE)) {
				Vehicle VehicleHandle = PED::GET_VEHICLE_PED_IS_USING(player);
				PED::SET_PED_INTO_VEHICLE(Client, VehicleHandle, GET_FREE_SEAT(VehicleHandle));
			}
			else
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z + 3, 1, 0, 0, 1);
		}
	}
	else
	{
		DrawNotification("Player is not playing!");
	}
}

void TeleportToMe(Ped playerPed, Ped selectedPed)
{
	Vector3 MyPosition = ENTITY::GET_ENTITY_COORDS(playerPed, TRUE);
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, TRUE))
	{
		Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_IN(selectedPed, FALSE);
		RequestControl(selectedVehicle);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(selectedVehicle, MyPosition.x + 2, MyPosition.y, MyPosition.z, FALSE, FALSE, TRUE);
		DrawNotification("Teleported");
	}
	else
	{
		DrawNotification("Player isn't in a vehicle");
	}
}

void KickPlayer(Player player)
{
	if (NETWORK::NETWORK_IS_HOST())
	{
		NETWORK::NETWORK_SESSION_KICK_PLAYER(player);
		DrawNotification("Kicked");
	}
	else
	{
		DrawNotification("Only works If you're host of this session");
	}
}

void KickOutFromVehicle(Ped ped)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(ped, TRUE))
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);
		WEAPON::REMOVE_WEAPON_FROM_PED(ped, 0xFBAB5776);
	}
	else
	{
		DrawNotification("Player isn't in a vehicle");
	}
}

void AttackPlayerWithRandomPeds(Ped selectedPed)
{
	//function not working
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(selectedVehicle); //requesting permission to fuck up another player

																	 //Remove PED from vehicle and lock it up
		AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
		VEHICLE::SET_VEHICLE_DOORS_LOCKED_FOR_ALL_PLAYERS(selectedVehicle, true);
	}
	if (!ENTITY::IS_ENTITY_DEAD(selectedPed)) {
		for (int z = 0; z < 8; ++z)
		{
			Vector3 targetPos = ENTITY::GET_ENTITY_COORDS(selectedPed, true);
			//Hash pHash = rand() % 2 == 0 ? $("U_M_Y_pogo_01") : $("u_m_y_rsranger_01");
			//STREAMING::REQUEST_MODEL(pHash);
			//while (!STREAMING::HAS_MODEL_LOADED(pHash)) WAIT(0);
			//Ped createdPed = PED::CREATE_PED(130, pHash, targetPos.x + 1, targetPos.y + 1, targetPos.z, 1.0, false, true);
			Ped createdPed = PED::CREATE_RANDOM_PED( targetPos.x + rand()%5*(rand()%2==0?-1:1), 
				targetPos.y + rand() % 5 * (rand() % 2 == 0 ? -1 : 1),
				targetPos.z);
			if (!ENTITY::DOES_ENTITY_EXIST(createdPed))
				continue;
			if (ENTITY::IS_ENTITY_DEAD(createdPed))
				continue;
			RequestControl(createdPed);
			//AI::CLEAR_PED_TASKS_IMMEDIATELY(createdPed);
			if (PED::IS_PED_IN_ANY_VEHICLE(createdPed, true))
				AI::TASK_LEAVE_ANY_VEHICLE(createdPed, 0, 0);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(createdPed, true, true);
			PED::SET_PED_CAN_SWITCH_WEAPON(createdPed, true);
			PED::SET_PED_COMBAT_ABILITY(createdPed, 2);
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(createdPed, 1);
			PED::SET_PED_CAN_RAGDOLL(createdPed,  0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(createdPed, 0);
			PED::SET_PED_COMBAT_RANGE(createdPed, 500);
			PED::SET_PED_ALERTNESS(createdPed, 100);
			PED::SET_PED_SHOOT_RATE(createdPed, 200);
			PED::SET_PED_KEEP_TASK(createdPed, 1);
			PED::ADD_ARMOUR_TO_PED(createdPed, 342);
			PED::SET_PED_ARMOUR(createdPed, 342);
			ENTITY::SET_ENTITY_INVINCIBLE(createdPed,true);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(createdPed, false);
			//WEAPON::GIVE_DELAYED_WEAPON_TO_PED(createdPed, WeaponStunGun, 1000, true);
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(createdPed, WeaponAssaultSMG, 1000, true);
			PED::SET_PED_RELATIONSHIP_GROUP_HASH(createdPed, $("cop"));
			PED::SET_PED_ACCURACY(createdPed,100);
			WEAPON::ADD_AMMO_TO_PED(createdPed, WeaponAssaultSMG, 1000);
			//WEAPON::GIVE_DELAYED_WEAPON_TO_PED(createdPed, 0x1E4A619F, 1000, true);
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(createdPed, WeaponRailgun, 1000, false);
			WEAPON::ADD_AMMO_TO_PED(createdPed, WeaponRailgun, 20);
			PED::REGISTER_TARGET(createdPed, selectedPed);
			Vector3 targetLoc = PED::GET_PED_BONE_COORDS(selectedPed, SKEL_Head, 0.0f, 0.0f, 0.0f);
			PED::SET_AI_WEAPON_DAMAGE_MODIFIER(10.0);
			PED::SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(10.0);
			//AI::TASK_AIM_GUN_AT_COORD(createdPed, targetLoc.x, targetLoc.y, targetLoc.z, 0.1, TRUE, TRUE);
			//PED::SET_PED_SHOOTS_AT_COORD(createdPed, targetLoc.x, targetLoc.y, targetLoc.z, true);
			
			//AI::TASK_GO_TO_COORD_ANY_MEANS(createdPed, targetPos.x, targetPos.y, targetPos.z, 1.0, 0, 0, 786603, 0xbf800000);
			//AI::TASK_PUT_PED_DIRECTLY_INTO_MELEE(createdPed, selectedPed, 0.0, -1.0, 0.0, 0);
			AI::TASK_COMBAT_PED(createdPed, selectedPed, 0, 16);
			//STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pHash);
		}

	}
	
}