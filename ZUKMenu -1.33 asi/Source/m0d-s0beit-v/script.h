#pragma once

void BypassOnlineModelRequestBlock(bool state);
void ScriptMain();
DWORD64 TunablesPointer();
void NOPAddress(DWORD64 address);

template <typename T> void SetTunable(int index, T value)
{
	/////not safe
	//static DWORD64 tunablesPointer = TunablesPointer();
	//*reinterpret_cast<T*>(tunablesPointer + (index+1) *8) = value;
}

template <typename T> T GetTunable(int index)
{
	//static DWORD64 tunablesPointer = TunablesPointer();
	//return *reinterpret_cast<T*>(tunablesPointer + (index + 1) * 8);
}
void NoIntro();