#include "LocalPlayer.h"

#define PI 3.14159265358

LocalPlayer* LocalPlayer::Get() {
	static uint32_t clientModule = (uint32_t)(GetModuleHandle(L"client.dll")); //parnorama?
	static LocalPlayer* localPlayer = (LocalPlayer*)(clientModule + offsets::dwLocalPlayer);

	return localPlayer;
}

Vector3* LocalPlayer::GetOrigin() {
	return (Vector3*)(*(uintptr_t*)this + offsets::m_vecOrigin);
}

Vector3* LocalPlayer::GetViewOffset() {
	return (Vector3*)(*(uintptr_t*)this + offsets::m_vecViewOffset);
}

int* LocalPlayer::GetHealth() {
	return (int*)(*(uintptr_t*)this + offsets::m_iHealth);
}

int* LocalPlayer::GetTeam() {
	return (int*)(*(uintptr_t*)this + offsets::m_iTeamNum);
}

float LocalPlayer::GetDistance(Vector3* other) {
	Vector3* myPos = GetOrigin();
	Vector3 delta = Vector3(other->x - myPos->x, other->y - myPos->y, other->z - myPos->z);

	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

void LocalPlayer::AimAt(Vector3* target) {
	static uint32_t engineModule = (uint32_t)GetModuleHandle(L"engine.dll");
	static Vector3* viewAngles = (Vector3*)(*(uint32_t*)(engineModule + offsets::dwClientState) + offsets::dwClientState_ViewAngles);

	Vector3 origin = *GetOrigin();
	Vector3 viewOffset = *GetViewOffset();
	Vector3* myPos = &(origin + viewOffset);

	Vector3 deltaVec = { target->x - myPos->x, target->y - myPos->y, target->z - myPos->z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = -asin(deltaVec.z / deltaVecLength) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);

	viewAngles->x = pitch;
	viewAngles->y = yaw;
}