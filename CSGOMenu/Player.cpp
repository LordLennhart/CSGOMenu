#include "Player.h"

Player* Player::GetPlayer(int index) {
  static uint32_t moduleBase =
      (uint32_t)GetModuleHandle(L"client_panorama.dll");
  static uint32_t entityList =
      moduleBase + hazedumper::signatures::dwEntityList;

  return (Player*)entityList + index * 0x10;
}

int* Player::GetMaxPlayer() {
  static uint32_t moduleBase = (uintptr_t)GetModuleHandle(L"engine.dll");

  return (int*)*(uint32_t*)moduleBase + hazedumper::signatures::dwClientState +
         hazedumper::signatures::dwClientState_MaxPlayer;
}

math::Vector3* Player::GetOrigin() {
  return (math::Vector3*)*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin;
}

math::Vector3* Player::GetViewOffset() {
  return (math::Vector3*)*(uintptr_t*)this +
         hazedumper::netvars::m_vecViewOffset;
}

math::Vector3 Player::GetBonePos(int boneId) {
  uint32_t boneMatrix =
      *(uint32_t*)*(uint32_t*)this + hazedumper::netvars::m_dwBoneMatrix;
  math::Vector3 bonePos;
  bonePos.x = *(float*)boneMatrix + 0x30 * boneId + 0x0C;
  bonePos.y = *(float*)boneMatrix + 0x30 * boneId + 0x1C;
  bonePos.z = *(float*)boneMatrix + 0x30 * boneId + 0x2C;

  return bonePos;
}

int* Player::GetHealth() {
  return (int*)*(uint32_t*)this + hazedumper::netvars::m_iHealth;
}

int* Player::GetTeam() {
  return (int*)*(uint32_t*)this + hazedumper::netvars::m_iTeamNum;
}