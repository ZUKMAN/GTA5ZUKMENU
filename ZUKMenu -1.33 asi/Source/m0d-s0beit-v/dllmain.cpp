#include "stdafx.h"
#include "inc\main.h"
#include "script.h"


MODULEINFO g_MainModuleInfo = { 0 };
DWORD64 GetModuleBase(HANDLE hProc, std::wstring &sModuleName)
{
	HMODULE *hModules;
	hModules = 0;
	wchar_t szBuf[50];
	DWORD cModules = 0;
	DWORD64 dwBase = 0;

	EnumProcessModules(hProc, hModules, 0, &cModules);
	hModules = new HMODULE[cModules / sizeof(HMODULE)];

	if (EnumProcessModules(hProc, hModules, cModules / sizeof(HMODULE), &cModules)) {
		for (int i = 0; i < cModules / sizeof(HMODULE); ++i) {
			if (GetModuleBaseName(hProc, hModules[i], szBuf, sizeof(szBuf))) {
				if (sModuleName.compare(szBuf) == 0) {
					dwBase = (DWORD64)hModules[i];
					break;
				}
			}
		}
	}
	return dwBase;
}

void bypass_online()
{
	__int64 Address = GetModuleBase(GetCurrentProcess(), std::wstring(L"GTA5.exe"));

	CHAR *MemoryBuff = new CHAR[4096];
	HANDLE hProcess = GetCurrentProcess();
	BYTE bytes[10] = { 0x48, 0x8B, 0x88, 0x10, 0x01, 0x00, 0x00, 0x48, 0x8B, 0xC1 };//BYPASS_SPAWN
	BYTE bytes2[10] = { 0x83, 0xBB, 0x34, 0x01, 0x00, 0x00, 0xFF, 0x0F, 0x95, 0xC0 };//BYPASS_EVERY_TIME_CHECK_FOR_PLAYER_MODEL
	BYTE nop2[2] = { 0x90, 0x90 };
	BYTE nop3[3] = { 0x90, 0x90, 0x90 };

	int Check = 0;
	for (;;)
	{
		ReadProcessMemory(hProcess, (LPVOID)Address, (LPVOID)MemoryBuff, 4096, NULL);
		for (INT p = 0; p < 4096; p++)
		{
			Address++;
			MemoryBuff++;
			if (memcmp(MemoryBuff, bytes, 10) == 0)
			{
				WriteProcessMemory(hProcess, (LPVOID)(Address + 0x20), nop2, 2, NULL);
				WriteProcessMemory(hProcess, (LPVOID)(Address + 0x2D), nop2, 2, NULL);
				Check += 1;
			}
			
			if (memcmp(MemoryBuff, bytes2, 10) == 0)
			{
				WriteProcessMemory(hProcess, (LPVOID)(Address - 3), nop3, 3, NULL);
				Check += 1;
			}
			if (Check >= 2)
			{
				goto endfunc;
			}
		}
		MemoryBuff = MemoryBuff - 4096;
	}
endfunc:;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	BOOL bReturnValue = TRUE;
	//LPWSTR proc64 = L"GTA5.exe";
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		//Log::Init(hModule);
		if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &g_MainModuleInfo, sizeof(g_MainModuleInfo)))
		{
			//Log::Fatal("Unable to get MODULEINFO from GTA5.exe");d
		}
		////NoIntro();//probably noy safe after 1.33
		scriptRegister(hinstDLL, ScriptMain);
		//bypass_online();//probably noy safe after 1.33
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hinstDLL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return bReturnValue;
}
