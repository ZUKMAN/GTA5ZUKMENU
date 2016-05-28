#include "../stdafx.h"

void BruteForceWeaponAddons(Ped ped, Hash weaponHash, bool bSilencer)
{
	//Since only I get these anyway, might as well craft it for me.
	const static Hash weaponAddons[] = { COMPONENT_AT_SCOPE_MACRO, COMPONENT_AT_SCOPE_MACRO_02, COMPONENT_AT_SCOPE_SMALL, COMPONENT_AT_SCOPE_SMALL_02, COMPONENT_AT_SCOPE_MEDIUM, COMPONENT_AT_SCOPE_LARGE, COMPONENT_AT_SCOPE_MAX, COMPONENT_AT_RAILCOVER_01, COMPONENT_AT_AR_AFGRIP, COMPONENT_AT_PI_FLSH, COMPONENT_AT_AR_FLSH, COMPONENT_PISTOL_CLIP_02, COMPONENT_COMBATPISTOL_CLIP_02, COMPONENT_APPISTOL_CLIP_02, COMPONENT_MICROSMG_CLIP_02, COMPONENT_SMG_CLIP_02, COMPONENT_ASSAULTRIFLE_CLIP_02, COMPONENT_CARBINERIFLE_CLIP_02, COMPONENT_ADVANCEDRIFLE_CLIP_02, COMPONENT_MG_CLIP_02, COMPONENT_COMBATMG_CLIP_02, COMPONENT_ASSAULTSHOTGUN_CLIP_02, COMPONENT_PISTOL50_CLIP_02, COMPONENT_ASSAULTSMG_CLIP_02, COMPONENT_SNSPISTOL_CLIP_02, COMPONENT_COMBATPDW_CLIP_02, COMPONENT_HEAVYPISTOL_CLIP_02, COMPONENT_SPECIALCARBINE_CLIP_02, COMPONENT_BULLPUPRIFLE_CLIP_02, COMPONENT_VINTAGEPISTOL_CLIP_02, COMPONENT_MARKSMANRIFLE_CLIP_02, COMPONENT_HEAVYSHOTGUN_CLIP_02, COMPONENT_GUSENBERG_CLIP_02 };

	for each (Hash addonHash in weaponAddons)
	{
		if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weaponHash, addonHash)) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, addonHash);
	}
	if (bSilencer)
	{
		//static Hash silencers[] = { COMPONENT_AT_PI_SUPP, COMPONENT_AT_PI_SUPP_02, COMPONENT_AT_AR_SUPP, COMPONENT_AT_SR_SUPP, COMPONENT_AT_AR_SUPP_02 };
		static Hash silencers[] = { COMPONENT_AT_PI_SUPP_02, COMPONENT_AT_AR_SUPP };
		for each (Hash silencerHash in silencers)
		{
			if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weaponHash, silencerHash))
			{
				if (weaponHash != WEAPON_ADVANCEDRIFLE && WEAPON::GET_WEAPONTYPE_GROUP(weaponHash) != WEAPON_TYPE_GROUP_SHOTGUN) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, silencerHash);
			}
		}
		if (weaponHash == WEAPON_SNIPERRIFLE || weaponHash == WEAPON_MICROSMG) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, COMPONENT_AT_AR_SUPP_02);
	}
}

void GiveAllWeapons(Ped ped)
{
	const static Hash weaponList[] = { WEAPON_APPISTOL, WEAPON_ASSAULTSHOTGUN, WEAPON_BAT, WEAPON_COMBATMG,
		WEAPON_COMBATPDW, WEAPON_CROWBAR, WEAPON_FIREWORK,
		WEAPON_FLAREGUN, WEAPON_GOLFCLUB, WEAPON_GRENADE, WEAPON_GRENADELAUNCHER,
		WEAPON_GUSENBERG, WEAPON_HEAVYSHOTGUN, WEAPON_HEAVYSNIPER, WEAPON_HOMINGLAUNCHER,
		WEAPON_MARKSMANRIFLE, WEAPON_MG, WEAPON_MICROSMG, WEAPON_MOLOTOV, WEAPON_MUSKET,
		WEAPON_PETROLCAN, WEAPON_PISTOL50, WEAPON_PROXMINE, WEAPON_RPG,
		WEAPON_SMOKEGRENADE, WEAPON_SNIPERRIFLE, WEAPON_STICKYBOMB,
		WEAPON_STUNGUN, WEAPON_MINIGUN, WEAPON_BZGAS, WEAPON_RAILGUN, WEAPON_FLARE,
		WEAPON_MARKSMANPISTOL, WEAPON_REMOTESNIPER, WEAPON_FIREEXTINGUISHER, WEAPON_FLASHLIGHT, WEAPON_MACHETE,
		WEAPON_MACHINEPISTOL, WEAPON_CARBINERIFLE, WEAPON_SNOWBALL, WEAPON_KNUCKLE };
	for each (Hash weapon in weaponList)
	{
		if (WEAPON::HAS_PED_GOT_WEAPON(ped, weapon, FALSE) == FALSE)
		{
			//WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, WEAPON::GET_MAX_AMMO(ped, weapon, &maxAmmo) == TRUE ? maxAmmo : 9999, FALSE, TRUE);
			WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, 9999, FALSE, TRUE);
			BruteForceWeaponAddons(ped, weapon, true); //This doesn't work for people who are not the player running the commands. You can take their weapons, but if you try to add attachments? FUCK YOU! I AIIIIIIIINNN'T HAVIN' THAT SHIT!
			WEAPON::SET_PED_WEAPON_TINT_INDEX(ped, weapon, WEAPONTINT_GOLD);
		}
		else
		{
			WEAPON::SET_PED_AMMO(ped, weapon, 9999);
		}
		if (WEAPON::GET_WEAPONTYPE_GROUP(weapon) == WEAPON_TYPE_GROUP_THROWABLE)
		{
			WEAPON::REMOVE_WEAPON_FROM_PED(ped, weapon);
			//WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, WEAPON::GET_MAX_AMMO(ped, weapon, &maxAmmo) == TRUE ? maxAmmo : 9999, FALSE, TRUE);
			WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, 9999, FALSE, TRUE);
		}
	}
}

void ReplenishAmmo(Ped playerPed)
{
	Hash currentWeapon;
	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, 1))
	{
		if (WEAPON::IS_WEAPON_VALID(currentWeapon))
		{
			int maxAmmo;
			if (WEAPON::GET_MAX_AMMO(playerPed, currentWeapon, &maxAmmo))
			{
				WEAPON::SET_PED_AMMO(playerPed, currentWeapon, maxAmmo);
			}
		}
	}
}

void InfiniteAmmo()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (ENTITY::DOES_ENTITY_EXIST(playerPed))
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);
					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
				}
			}
		}
	}
}

void sprayBullet(Vector3 * target)
{
	if (!target)
		return;
	if (rand() % 2) {
		target->x += -0.5 + (rand() / RAND_MAX) * 1;
		srand(GetTickCount());
	}
	if (rand() % 2) {
		target->y += -0.5 + (rand() / RAND_MAX) * 1;
		srand(GetTickCount());
	}
	if (rand() % 2)
		target->z += -0.5 + (rand() / RAND_MAX)*1;
}

int randomBone(std::vector<int> * bones)
{
	if (!bones || bones->empty())
		return SKEL_Head;
	srand(GetTickCount());
	return bones->operator[](rand() % bones->size());
}

void OneHitKillEnable(Player PlayerPed, bool state)
{
	if (state) {
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PlayerPed, 999999);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PlayerPed, 999999);
	}	
	else {
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PlayerPed, 1);
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PlayerPed, 1);
	}
		
}
