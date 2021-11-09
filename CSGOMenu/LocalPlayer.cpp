#include "LocalPlayer.h"


LocalPlayer* LocalPlayer::Get() {
	static uint32_t clientModule = (uint32_t)GetModuleHandle(L"client_panorama.dll");
	static LocalPlayer* localPlayer = (LocalPlayer*)clientModule + hazedumper::signatures::dwLocalPlayer;

	return localPlayer;
}

math::Vector3* LocalPlayer::GetOrigin() {
	return (math::Vector3*)*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin;
}

math::Vector3* LocalPlayer::GetViewOffset() {
	return (math::Vector3*)*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset;
}

int* LocalPlayer::GetHealth() {
	return (int*) * (uintptr_t*)this + hazedumper::netvars::m_iHealth;
}

int* LocalPlayer::GetTeam() {
	return (int*)*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum;
}