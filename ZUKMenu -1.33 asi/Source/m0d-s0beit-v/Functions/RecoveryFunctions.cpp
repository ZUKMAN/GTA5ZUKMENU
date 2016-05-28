#include "../stdafx.h"


void EnableVehiclePurchase(Hash hash)
{
	SetStat("MPPLY_VEHICLE_ID_ADMIN_WEB", hash);
}

void RedesignCharacter()
{
	STATS::STAT_SET_BOOL($("MP0_FM_CHANGECHAR_ASKED"), 0, 1);
	DrawNotification("Allowed Redesign Character");
}

void MaxSkills()
{
	SetStatMax("MP0_SCRIPT_INCREASE_STAM", 100);
	SetStatMax("MP0_SCRIPT_INCREASE_STRN", 100);
	SetStatMax("MP0_SCRIPT_INCREASE_LUNG", 100);
	SetStatMax("MP0_SCRIPT_INCREASE_FLY", 100);
	SetStatMax("MP0_SCRIPT_INCREASE_DRIV", 100);
	SetStatMax("MP0_SCRIPT_INCREASE_SHO", 100);
	SetStatMax("MP0_SCRIPT_INCREASE_STL", 100);
	DrawNotification("Set Max Skills");
}

void UnlockChromeRims()
{
	SetStatMax("MP0_AWD_WIN_CAPTURES", rand() % 25 + 25);
	SetStatMax("MP0_AWD_DROPOFF_CAP_PACKAGES", rand() % 50 + 100);
	SetStatMax("MP0_AWD_KILL_CARRIER_CAPTURE", rand() % 50 + 100);
	SetStatMax("MP0_AWD_FINISH_HEISTS", rand() % 25 + 50);
	SetStatMax("MP0_AWD_FINISH_HEIST_SETUP_JOB", rand() % 25 + 50);
	SetStatMax("MP0_AWD_NIGHTVISION_KILLS", rand() % 50 + 100);
	SetStatMax("MP0_AWD_WIN_LAST_TEAM_STANDINGS", rand() % 25 + 50);
	SetStatMax("MP0_AWD_ONLY_PLAYER_ALIVE_LTS", rand() % 25 + 50);
	DrawNotification("Unlocked Chrome Rims");
}

void UnlockAllWeapons()
{
	SetStat("MP0_ADMIN_WEAPON_GV_BS_1", -1);
	SetStat("MP0_ADMIN_WEAPON_GV_BS_2", -1);
	SetStat("MP0_ADMIN_WEAPON_GV_BS_3", -1);
	SetStat("MP0_BOTTLE_IN_POSSESSION", -1);
	SetStat("MP0_CHAR_FM_WEAP_UNLOCKED", -1);
	SetStat("MP0_CHAR_FM_WEAP_UNLOCKED2", -1);
	SetStat("MP0_CHAR_WEAP_FM_PURCHASE", -1);
	SetStat("MP0_CHAR_WEAP_FM_PURCHASE2", -1);
	SetStat("MP0_CHAR_FM_WEAP_ADDON_1_UNLCK", -1);
	SetStat("MP0_CHAR_FM_WEAP_ADDON_2_UNLCK", -1);
	SetStat("MP0_CHAR_FM_WEAP_ADDON_3_UNLCK", -1);
	SetStat("MP0_CHAR_FM_WEAP_ADDON_4_UNLCK", -1);
	SetStat("MP0_CHAR_FM_WEAP_ADDON_5_UNLCK", -1);
	SetStat("MP0_WEAP_FM_ADDON_PURCH", -1);
	SetStat("MP0_WEAP_FM_ADDON_PURCH2", -1);
	SetStat("MP0_WEAP_FM_ADDON_PURCH3", -1);
	SetStat("MP0_WEAP_FM_ADDON_PURCH4", -1);
	SetStat("MP0_WEAP_FM_ADDON_PURCH5", -1);
	DrawNotification("Unlocked All Weapons");
}

void UnlockHeistVehicles()
{
	SetStat("MP0_CHAR_KIT_FM_PURCHASE", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE2", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE3", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE4", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE5", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE6", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE7", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE8", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE9", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE10", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE11", -1);
	SetStat("MP0_CHAR_KIT_FM_PURCHASE12", -1);
	SetStat("MP0_CHAR_KIT_1_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_2_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_3_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_4_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_5_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_6_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_7_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_8_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_9_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_10_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_11_FM_UNLCK", -1);
	SetStat("MP0_CHAR_KIT_12_FM_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_0_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_1_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_2_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_3_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_4_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_5_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_6_UNLCK", -1);
	SetStat("MP0_CHAR_FM_CARMOD_7_UNLCK", -1);
	SetStat("MP0_CHAR_FM_VEHICLE_1_UNLCK", -1);
	SetStat("MP0_CHAR_FM_VEHICLE_2_UNLCK", -1);
	SetStat("MP0_CHAR_FM_ABILITY_1_UNLCK", -1);
	SetStat("MP0_CHAR_FM_ABILITY_2_UNLCK", -1);
	SetStat("MP0_CHAR_FM_ABILITY_3_UNLCK", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_1_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_2_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_3_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_4_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_5_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_6_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_7_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_8_COLLECT", -1);
	SetStat("MP0_CHAR_FM_PACKAGE_9_COLLECT", -1);
	SetStat("MP0_CHAR_FM_HEALTH_1_UNLCK", -1);
	SetStat("MP0_CHAR_FM_HEALTH_2_UNLCK", -1);
	SetStat("MP0_HOLDUPS_BITSET", -1);

	DrawNotification("Unlocked Heist Vehicles");
}

void UnlockLSC()
{
	SetStatMax("MP0_AWD_ENEMYDRIVEBYKILLS", rand() % 100 + 600);
	SetStatMax("MP0_USJS_COMPLETED", 50);
	SetStatMax("MP0_USJS_FOUND", 50);
	SetStatMax("MP0_AWD_FM_TDM_MVP", rand() % 25 + 60);
	SetStatMax("MP0_AWD_HOLD_UP_SHOPS", rand() % 25 + 25);
	SetStatMax("MP0_AWD_RACES_WON", rand() % 25 + 75);
	SetStatMax("MP0_AWD_NO_ARMWRESTLING_WINS", rand() % 25 + 25);
	SetStatMax("MP0_AWD_FMBBETWIN", rand() % 10000 + 25000);
	SetStatMax("MP0_AWD_FM_DM_WINS", rand() % 25 + 25);
	SetStatMax("MP0_AWD_FM_DM_TOTALKILLS", rand() % 25 + 500);
	SetStatMax("MP0_MPPLY_DM_TOTAL_DEATHS", rand() % 50 + 400);
	SetStatMax("MP0_MPPLY_TIMES_FINISH_DM_TOP_3", rand() % 25 + 25);
	SetStatMax("MP0_PLAYER_HEADSHOTS", rand() % 25 + 200);
	SetStatMax("MP0_AWD_FM_DM_WINS", rand() % 25 + 50);
	SetStatMax("MP0_AWD_FM_TDM_WINS", rand() % 25 + 20);
	SetStatMax("MP0_AWD_FM_GTA_RACES_WON", rand() % 5 + 10);
	SetStatMax("MP0_AWD_FM_GOLF_WON", rand() % 5 + 2);
	SetStatMax("MP0_AWD_FM_SHOOTRANG_TG_WON", rand() % 4 + 2);
	SetStatMax("MP0_AWD_FM_SHOOTRANG_RT_WON", rand() % 4 + 2);
	SetStatMax("MP0_AWD_FM_SHOOTRANG_CT_WON", rand() % 4 + 2);
	SetStatMax("MP0_AWD_FM_SHOOTRANG_GRAN_WON", rand() % 4 + 2);
	SetStatMax("MP0_AWD_FM_TENNIS_WON", rand() % 4 + 2);
	SetStatMax("MP0_MPPLY_TENNIS_MATCHES_WON", rand() % 4 + 2);
	SetStatMax("MP0_MPPLY_TOTAL_TDEATHMATCH_WON", rand() % 25 + 50);
	SetStatMax("MP0_MPPLY_TOTAL_RACES_WON", rand() % 75 + 200);
	SetStatMax("MP0_MPPLY_TOTAL_DEATHMATCH_LOST", rand() % 25 + 25);
	SetStatMax("MP0_MPPLY_TOTAL_RACES_LOST", rand() % 25 + 25);
	SetStatMax("MP0_number_turbo_starts_in_race", rand() % 25 + 50);
	SetStatMax("MP0_awd_fmwinairrace", rand() % 25 + 25);
	SetStatMax("MP0_awd_fmwinsearace", rand() % 25 + 25);
	SetStatMax("MP0_awd_fmrallywonnav", rand() % 25 + 25);
	SetStatMax("MP0_awd_fmrallywondrive ", rand() % 25 + 25);
	SetStatMax("MP0_awd_fm_races_fastest_lap ", rand() % 250 + 250);

	DrawNotification("Unlocked LSC");
}

void SetTotalRP(int rp)
{
	SetStatMax("MP0_CHAR_XP_FM", rp);
	//SetStatMax("MP1_CHAR_XP_FM", rp);
	//SetStatMax("MP2_CHAR_XP_FM", rp);
}

void RedesignCharacter2()
{
	STATS::STAT_SET_BOOL($("MP1_FM_CHANGECHAR_ASKED"), 0, 1);
	DrawNotification("Allowed Redesign Character");
}

void MaxSkills2()
{
	SetStatMax("MP1_SCRIPT_INCREASE_STAM", 100);
	SetStatMax("MP1_SCRIPT_INCREASE_STRN", 100);
	SetStatMax("MP1_SCRIPT_INCREASE_LUNG", 100);
	SetStatMax("MP1_SCRIPT_INCREASE_FLY", 100);
	SetStatMax("MP1_SCRIPT_INCREASE_DRIV", 100);
	SetStatMax("MP1_SCRIPT_INCREASE_SHO", 100);
	SetStatMax("MP1_SCRIPT_INCREASE_STL", 100);
	DrawNotification("Set Max Skills");
}

void UnlockChromeRims2()
{
	SetStatMax("MP1_AWD_WIN_CAPTURES", rand() % 25 + 25);
	SetStatMax("MP1_AWD_DROPOFF_CAP_PACKAGES", rand() % 50 + 100);
	SetStatMax("MP1_AWD_KILL_CARRIER_CAPTURE", rand() % 50 + 100);
	SetStatMax("MP1_AWD_FINISH_HEISTS", rand() % 25 + 50);
	SetStatMax("MP1_AWD_FINISH_HEIST_SETUP_JOB", rand() % 25 + 50);
	SetStatMax("MP1_AWD_NIGHTVISION_KILLS", rand() % 50 + 100);
	SetStatMax("MP1_AWD_WIN_LAST_TEAM_STANDINGS", rand() % 25 + 50);
	SetStatMax("MP1_AWD_ONLY_PLAYER_ALIVE_LTS", rand() % 25 + 50);
	DrawNotification("Unlocked Chrome Rims");
}

void UnlockAllWeapons2()
{
	SetStat("MP1_ADMIN_WEAPON_GV_BS_1", -1);
	SetStat("MP1_ADMIN_WEAPON_GV_BS_2", -1);
	SetStat("MP1_ADMIN_WEAPON_GV_BS_3", -1);
	SetStat("MP1_BOTTLE_IN_POSSESSION", -1);
	SetStat("MP1_CHAR_FM_WEAP_UNLOCKED", -1);
	SetStat("MP1_CHAR_FM_WEAP_UNLOCKED2", -1);
	SetStat("MP1_CHAR_WEAP_FM_PURCHASE", -1);
	SetStat("MP1_CHAR_WEAP_FM_PURCHASE2", -1);
	SetStat("MP1_CHAR_FM_WEAP_ADDON_1_UNLCK", -1);
	SetStat("MP1_CHAR_FM_WEAP_ADDON_2_UNLCK", -1);
	SetStat("MP1_CHAR_FM_WEAP_ADDON_3_UNLCK", -1);
	SetStat("MP1_CHAR_FM_WEAP_ADDON_4_UNLCK", -1);
	SetStat("MP1_CHAR_FM_WEAP_ADDON_5_UNLCK", -1);
	SetStat("MP1_WEAP_FM_ADDON_PURCH", -1);
	SetStat("MP1_WEAP_FM_ADDON_PURCH2", -1);
	SetStat("MP1_WEAP_FM_ADDON_PURCH3", -1);
	SetStat("MP1_WEAP_FM_ADDON_PURCH4", -1);
	SetStat("MP1_WEAP_FM_ADDON_PURCH5", -1);
	DrawNotification("Unlocked All Weapons");
}

void UnlockHeistVehicles2()
{
	SetStat("MP1_CHAR_KIT_FM_PURCHASE", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE2", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE3", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE4", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE5", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE6", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE7", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE8", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE9", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE10", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE11", -1);
	SetStat("MP1_CHAR_KIT_FM_PURCHASE12", -1);
	SetStat("MP1_CHAR_KIT_1_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_2_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_3_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_4_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_5_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_6_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_7_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_8_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_9_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_10_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_11_FM_UNLCK", -1);
	SetStat("MP1_CHAR_KIT_12_FM_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_0_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_1_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_2_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_3_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_4_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_5_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_6_UNLCK", -1);
	SetStat("MP1_CHAR_FM_CARMOD_7_UNLCK", -1);
	SetStat("MP1_CHAR_FM_VEHICLE_1_UNLCK", -1);
	SetStat("MP1_CHAR_FM_VEHICLE_2_UNLCK", -1);
	SetStat("MP1_CHAR_FM_ABILITY_1_UNLCK", -1);
	SetStat("MP1_CHAR_FM_ABILITY_2_UNLCK", -1);
	SetStat("MP1_CHAR_FM_ABILITY_3_UNLCK", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_1_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_2_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_3_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_4_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_5_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_6_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_7_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_8_COLLECT", -1);
	SetStat("MP1_CHAR_FM_PACKAGE_9_COLLECT", -1);
	SetStat("MP1_CHAR_FM_HEALTH_1_UNLCK", -1);
	SetStat("MP1_CHAR_FM_HEALTH_2_UNLCK", -1);
	SetStat("MP1_HOLDUPS_BITSET", -1);

	DrawNotification("Unlocked Heist Vehicles");
}

void UnlockLSC2()
{
	SetStatMax("MP1_AWD_ENEMYDRIVEBYKILLS", rand() % 100 + 600);
	SetStatMax("MP1_USJS_COMPLETED", 50);
	SetStatMax("MP1_USJS_FOUND", 50);
	SetStatMax("MP1_AWD_FM_TDM_MVP", rand() % 25 + 60);
	SetStatMax("MP1_AWD_HOLD_UP_SHOPS", rand() % 25 + 25);
	SetStatMax("MP1_AWD_RACES_WON", rand() % 25 + 75);
	SetStatMax("MP1_AWD_NO_ARMWRESTLING_WINS", rand() % 25 + 25);
	SetStatMax("MP1_AWD_FMBBETWIN", rand() % 10000 + 25000);
	SetStatMax("MP1_AWD_FM_DM_WINS", rand() % 25 + 25);
	SetStatMax("MP1_AWD_FM_DM_TOTALKILLS", rand() % 25 + 500);
	SetStatMax("MP1_MPPLY_DM_TOTAL_DEATHS", rand() % 50 + 400);
	SetStatMax("MP1_MPPLY_TIMES_FINISH_DM_TOP_3", rand() % 25 + 25);
	SetStatMax("MP1_PLAYER_HEADSHOTS", rand() % 25 + 200);
	SetStatMax("MP1_AWD_FM_DM_WINS", rand() % 25 + 50);
	SetStatMax("MP1_AWD_FM_TDM_WINS", rand() % 25 + 20);
	SetStatMax("MP1_AWD_FM_GTA_RACES_WON", rand() % 5 + 10);
	SetStatMax("MP1_AWD_FM_GOLF_WON", rand() % 5 + 2);
	SetStatMax("MP1_AWD_FM_SHOOTRANG_TG_WON", rand() % 4 + 2);
	SetStatMax("MP1_AWD_FM_SHOOTRANG_RT_WON", rand() % 4 + 2);
	SetStatMax("MP1_AWD_FM_SHOOTRANG_CT_WON", rand() % 4 + 2);
	SetStatMax("MP1_AWD_FM_SHOOTRANG_GRAN_WON", rand() % 4 + 2);
	SetStatMax("MP1_AWD_FM_TENNIS_WON", rand() % 4 + 2);
	SetStatMax("MP1_MPPLY_TENNIS_MATCHES_WON", rand() % 4 + 2);
	SetStatMax("MP1_MPPLY_TOTAL_TDEATHMATCH_WON", rand() % 25 + 50);
	SetStatMax("MP1_MPPLY_TOTAL_RACES_WON", rand() % 75 + 200);
	SetStatMax("MP1_MPPLY_TOTAL_DEATHMATCH_LOST", rand() % 25 + 25);
	SetStatMax("MP1_MPPLY_TOTAL_RACES_LOST", rand() % 25 + 25);
	SetStatMax("MP1_number_turbo_starts_in_race", rand() % 25 + 50);
	SetStatMax("MP1_awd_fmwinairrace", rand() % 25 + 25);
	SetStatMax("MP1_awd_fmwinsearace", rand() % 25 + 25);
	SetStatMax("MP1_awd_fmrallywonnav", rand() % 25 + 25);
	SetStatMax("MP1_awd_fmrallywondrive ", rand() % 25 + 25);
	SetStatMax("MP1_awd_fm_races_fastest_lap ", rand() % 250 + 250);

	DrawNotification("Unlocked LSC");
}

void SetTotalRP2(int rp)
{
	SetStatMax("MP1_CHAR_XP_FM", rp);
	//SetStatMax("MP1_CHAR_XP_FM", rp);
	//SetStatMax("MP2_CHAR_XP_FM", rp);
}

void WantedLevelLoop(Player player)
{
	static int i = 0;

	if (i % 2 == 0)
	{
		if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) == 0)
		{
			PLAYER::REPORT_CRIME(player, 36, PLAYER::GET_WANTED_LEVEL_THRESHOLD(5));
			PLAYER::SET_WANTED_LEVEL_DIFFICULTY(player, 0.0f);
			PLAYER::SET_PLAYER_WANTED_CENTRE_POSITION(player, -99999, -99999, 99999);
			PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(player, FALSE);
		}
		else
		{
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
		}
	}
	i++;
}

void SetStatMax(char* hashname, int value)
{
	Hash hash = $(hashname);
	int current = 0;
	if (STATS::STAT_GET_INT(hash, &current, -1))
	{
		STATS::STAT_SET_INT(hash, max(current, value), FALSE);
	}
}

void SetStat(char* hashname, int value)
{
	STATS::STAT_SET_INT($(hashname), value, TRUE);
}

