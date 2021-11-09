#include "Player.h"

Player* Player::GetPlayer(int index) {
	static uint32_t moduleBase = (uint32_t)GetModuleHandle(L"client_panorama.dll");
	static uint32_t entityList = moduleBase + hazedumper::signatures::dwEntityList;

	return (Player*)entityList + index * 0x10;
}

int* Player::GetMaxPlayer() {
	static uint32_t moduleBase = (uintptr_t)GetModuleHandle(L"engine.dll");
	
	return (int*)*(uint32_t*)moduleBase + hazedumper::signatures::dwClientState + hazedumper::signatures::dwClientState_MaxPlayer;
}

math::Vector3* Player::GetOrigin() {
	return (math::Vector3*)*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin;
}

math::Vector3* Player::GetViewOffset() {
	return (math::Vector3*)*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset;
}