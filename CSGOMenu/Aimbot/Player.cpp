#include "Player.h"

int Player::playerIndex = -1;

Player* Player::GetPlayer(int index) {
	static uint32_t moduleBase = (uint32_t)GetModuleHandle(L"client.dll");
	static uint32_t entityList = moduleBase + offsets::dwEntityList;
	playerIndex = index;
	return (Player*)(entityList + index * 0x10);
}

int* Player::GetMaxPlayer() {
	static uint32_t moduleBase = (uintptr_t)GetModuleHandle(L"engine.dll");

	return (int*)(*(uint32_t*)(moduleBase + offsets::dwClientState) + offsets::dwClientState_MaxPlayer);
}

Vector3* Player::GetOrigin() {
	return (Vector3*)(*(uintptr_t*)this + offsets::m_vecOrigin);
}

Vector3* Player::GetViewOffset() {
	return (Vector3*)(*(uintptr_t*)this + offsets::m_vecViewOffset);
}

Vector3* Player::GetBonePos(int boneId) {
	uint32_t boneMatrix = *(uint32_t*)(*(uint32_t*)this + offsets::m_dwBoneMatrix);
	static Vector3 bonePos;
	bonePos.x = *(float*)(boneMatrix + 0x30 * boneId + 0x0C);
	bonePos.y = *(float*)(boneMatrix + 0x30 * boneId + 0x1C);
	bonePos.z = *(float*)(boneMatrix + 0x30 * boneId + 0x2C);

	return &bonePos;
}

int* Player::GetHealth() {
	return (int*)(*(uint32_t*)this + offsets::m_iHealth);
}
int* Player::GetTeam() {
	return (int*)(*(uint32_t*)this + offsets::m_iTeamNum);
}

bool Player::SeenBy() {
	int* ptr = (int*)(uint32_t*)((offsets::m_bSpottedByMask & (0x1 << playerIndex)));
	std::cout << ptr << std::endl;
	return ((offsets::m_bSpottedByMask & (0x1 << playerIndex)) != 0);
}
//bool Player::SeenBy(Player* player) {
//	return SeenBy()
//}

