#pragma once
#include <math.h>
typedef struct
{
	int red;
	int green;
	int blue;
} Color;

typedef struct
{
	char* name;
	Hash hash;
	int wheelType;
	std::vector<std::pair<int, int>> mods;
	std::vector<int> toggles;
	std::vector<std::pair<int, BOOL>> extras;
	char* plateText;
	int plateType;
	int windowType;
	Color tireSmoke;
	std::vector<std::pair<int, BOOL>> neons;
	Color neonColor;
	int primaryColor;
	int secondaryColor;
	int pearlColor;
	int wheelColor;
	BOOL customPrimary;
	BOOL customSecondary;
	Color primaryRGB;
	Color secondaryRGB;
	int dashBoardColor;
	int interiorColor;
} SavedVehicle;

void BypassOnlineVehicleKick(Vehicle vehicle, Player player);
void BoostBaseVehicleStats(Vehicle vehicle, bool notification);
void DumpPlayerFromVehicle(Ped PlayerID, bool notification);
void SpawnPlayerVehicle(Ped playerPed, Hash vehicleHash, bool spawnInVehicle, bool baseUpgrade, bool maxUpgrade, bool preventKick);
void SpawnSavedVehicle(Ped playerPed, SavedVehicle savedVehicle, bool spawnInVehicle, bool preventKick);
SavedVehicle SaveVehicle(char* name, Vehicle vehicle);
Menu SavedMenu(std::vector<SavedVehicle> savedVehicles, Ped selfPed, bool* spawnInVehicle, bool preventKick);
void SetPlateType(Vehicle vehicle, PlateTypes plateType);
void SetWindowTint(Vehicle vehicle, WindowTints tint);
void ResprayVehicle(Vehicle vehicle, int primary, int secondary, int pearl, int wheel);
void SetVehiclePrimary(Vehicle vehicle, int color);
void SetVehicleSecondary(Vehicle vehicle, int color);
void SetVehiclePearl(Vehicle vehicle, int color);
void SetVehicleWheelColor(Vehicle vehicle, int color);
void ToggleExtra(Vehicle vehicle, int extraID);
void ClaimVehicle(Vehicle vehicle);
void SetOffAlarmofPlayerVehicle(Ped selectedPed);
void AIJackVehicle(Ped selectedPlayer);
Vehicle spawnMoneyVehicle(Ped selfPed, Ped selectedPlayer);
void SetNumberPlate(Vehicle playerVeh, char* text);
void RepairVehicle(Vehicle vehicle);
void SetVehicleModded(Vehicle vehicle);
bool DoesVehicleExist(char* name);
void SetVehicleMod(Vehicle vehicle, int modType, int modIndex);
void ToggleVehicleMod(Vehicle vehicle, int modType);
void SetVehicleToggle(Vehicle vehicle, int modType, BOOL toggle);
void SetVehicleLivery(Vehicle vehicle, int livery);
void SetTireSmoke(Vehicle vehicle, int r, int g, int b);
void ToggleNeonLight(Vehicle vehicle, int index);
void MaxUpgrade(Vehicle vehicle);
void VehicleFly(Vehicle vehicle, float & yaw);
std::vector<std::pair<std::string, Menu>> VehicleCustomMenu(Vehicle vehicle);
void ToggleRadio(bool state);
void SaveVehiclesToDisk(std::vector<SavedVehicle> savedVehicles);
std::vector<SavedVehicle> LoadVehiclesFromDisk();
color_t makeColorGradient(int i, double frequency1 = 0.0125, double frequency2 = 0.0125, double frequency3 = 0.0125,
	double phase1 = 2 * M_PI / 3, double phase2 = 0, double phase3 = 4 * M_PI / 3,
	int center = 0, int width = 0, int len = 44);
int getClosestStandartColor(color_t * col);