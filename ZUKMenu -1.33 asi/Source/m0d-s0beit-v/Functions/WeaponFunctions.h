#pragma once

void BruteForceWeaponAddons(Ped ped, Hash weaponHash, bool bSilencer);
void GiveAllWeapons(Ped ped);
void ReplenishAmmo(Ped playerPed);
void InfiniteAmmo();
void OneHitKillEnable(Ped PlayerPed, bool state);
void sprayBullet(Vector3 * target);
int randomBone(std::vector<int> * bones);