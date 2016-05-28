#include "stdafx.h"

void ScriptMain()
{
	srand(GetTickCount());
	while (true) {
		UpdateMenu();
		WAIT(0);
	}

}


unsigned short oldSpanBypassValue =-1;

void BypassOnlineModelRequestBlock(bool state)
{	 
	DWORD64 dwGetModelTableFunctionAddress = Pattern::Scan(g_MainModuleInfo,   "48 8B 88 10 01 00 00 48 8B C1");
	//DWORD64 dwGetModelTableFunctionAddress = Pattern::Scan(g_MainModuleInfo, "48 8B C8 FF 52 30 84 C0 74 05 48");
	if (dwGetModelTableFunctionAddress != NULL)
	{
		if (oldSpanBypassValue == -1)
			oldSpanBypassValue = *reinterpret_cast<unsigned short*>(dwGetModelTableFunctionAddress + 0x8);
		*reinterpret_cast<unsigned short*>(dwGetModelTableFunctionAddress + 0x20) = state ? 0x9090 : oldSpanBypassValue;
		*reinterpret_cast<unsigned short*>(dwGetModelTableFunctionAddress + 0x2D) = state ? 0x9090 : oldSpanBypassValue;
	}
}

void NoIntro()
{
	DWORD64 dwRockstarLogo,	dwSplashScreen;

	do {
		dwRockstarLogo = Pattern::Scan(g_MainModuleInfo, "70 6C 61 74 66 6F 72 6D 3A");
	} while (dwRockstarLogo == NULL);
	NOPAddress(dwRockstarLogo);

	
	do {
		dwSplashScreen = Pattern::Scan(g_MainModuleInfo, "72 1F E8 ? ? ? ? 8B 0D");
	} while (dwSplashScreen == NULL);
	NOPAddress(dwSplashScreen);	
}

DWORD64 TunablesPointer()
{
	return *reinterpret_cast<DWORD64*>(DWORD64(g_MainModuleInfo.lpBaseOfDll) + *reinterpret_cast<int*>(Pattern::Scan(g_MainModuleInfo, "48 8B 8C C2 ? ? ? ? 48 85 C9 74 19") + 4) + 8);
}

void NOPAddress(DWORD64 address)
{
	memset(reinterpret_cast<void*>(address), 0x90, 2);
}