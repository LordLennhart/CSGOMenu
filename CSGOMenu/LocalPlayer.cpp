#include "LocalPlayer.h"

LocalPlayer* LocalPlayer::Get() {
  static uint32_t clientModule =
      (uint32_t)GetModuleHandle(L"client_panorama.dll");
  static LocalPlayer* localPlayer =
      (LocalPlayer*)clientModule + hazedumper::signatures::dwLocalPlayer;

  return localPlayer;
}

math::Vector3* LocalPlayer::GetOrigin() {
  return (math::Vector3*)*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin;
}

math::Vector3* LocalPlayer::GetViewOffset() {
  return (math::Vector3*)*(uintptr_t*)this +
         hazedumper::netvars::m_vecViewOffset;
}

int* LocalPlayer::GetHealth() {
  return (int*)*(uintptr_t*)this + hazedumper::netvars::m_iHealth;
}

int* LocalPlayer::GetTeam() {
  return (int*)*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum;
}

float LocalPlayer::GetDistance(math::Vector3* other) {
  math::Vector3* myPos = GetOrigin();
  math::Vector3 delta = math::Vector3(other->x - myPos->x, other->y - myPos->y,
                                      other->z - myPos->z);

  return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

void LocalPlayer::AimAt(math::Vector3 target) {
  Beep(4000, 400);
  static uint32_t engineModule = (uint32_t)GetModuleHandle(L"engine.dll");
  static math::Vector3* viewAngles =
      (math::Vector3*)*(uintptr_t*)(engineModule +
      hazedumper::signatures::dwClientState) +
      hazedumper::signatures::dwClientState_ViewAngles;
  math::Vector3 origin = *GetOrigin();
  math::Vector3 viewOffset = *GetViewOffset();
  math::Vector3* myPos = &(origin + viewOffset);

  math::Vector3 deltaVec = {target.x - myPos->x, target.y - myPos->y,
                            target.z - myPos->z};
  std::wstringstream ws;
  ws << "myPos: " << *myPos << "\n"
     << "deltaVec: " << deltaVec << "\n";

  float deltaVecLength =
      sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y +
           deltaVec.z * deltaVec.z);

  float pitch = -asin(deltaVec.z / deltaVecLength) * (180 / PI);
  float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);

  ws << "Yaw: " << yaw << "\n"
     << "Pitch: " << pitch;
  MessageBox(NULL, ws.str().c_str(), L"Hurensöhn", MB_OK | MB_SETFOREGROUND);


  viewAngles->x = pitch;
  viewAngles->y = yaw;
}