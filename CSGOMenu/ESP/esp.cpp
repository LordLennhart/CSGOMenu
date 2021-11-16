#include "esp.h"
#include "../includes.h"

Esp::ESP::~ESP() {
  this->FontF->Release();
  this->LineL->Release();
}

void Esp::ESP::Init() {
  client = (uintptr_t)GetModuleHandle(L"client.dll");
  engine = (uintptr_t)GetModuleHandle(L"engine.dll");
  entList = (Entity::EntList*)(client + offsets::dwEntityList);
  localEnt = entList->ents[0].ent;
}

void Esp::ESP::Update() {
  memcpy(&viewMatrix, (PBYTE*)(client + offsets::dwViewMatrix),
         sizeof(viewMatrix));
  this->CheckButtons();
}

bool Esp::ESP::CheckValidEnt(Entity::Ent* ent) {
  if (ent == nullptr) return false;
  if (ent == localEnt) return false;
  if (ent->iHealth <= 0) return false;
  if (ent->isDormant) return false;
  return true;
}

bool Esp::ESP::WorldToScreen(Vec3 pos, Vec2& screen) {
  Vec4 clipCoords;
  clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] +
                 pos.z * viewMatrix[2] + viewMatrix[3];
  clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] +
                 pos.z * viewMatrix[6] + viewMatrix[7];
  clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] +
                 pos.z * viewMatrix[10] + viewMatrix[11];
  clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] +
                 pos.z * viewMatrix[14] + viewMatrix[15];

  if (clipCoords.w < 0.1f) return false;

  Vec3 NDC;
  NDC.x = clipCoords.x / clipCoords.w;
  NDC.y = clipCoords.y / clipCoords.w;
  NDC.z = clipCoords.z / clipCoords.w;

  screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
  screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
  return true;
}

Vec3 Esp::ESP::GetBonePos(Entity::Ent* ent, int boneId) {
  uintptr_t bonePtr = ent->boneMatrix;
  Vec3 bonePos;
  bonePos.x = *(float*)(bonePtr + 0x30 * boneId + 0x0C);
  bonePos.y = *(float*)(bonePtr + 0x30 * boneId + 0x1C);
  bonePos.z = *(float*)(bonePtr + 0x30 * boneId + 0x2C);

  return bonePos;
}

Vec3 Esp::ESP::TransformVec(Vec3 src, Vec3 angl, float distance) {
  Vec3 newPos;
  newPos.x = src.x + (cosf(TORAD(angl.y)) * distance);
  newPos.y = src.y + (sinf(TORAD(angl.y)) * distance);
  newPos.z = src.z + (sinf(TORAD(angl.x)) * distance);

  return newPos;
}

void Esp::ESP::CheckButtons() {
  if (GetAsyncKeyState(VK_INSERT) & 1) settings.showMenu = !settings.showMenu;
  if (GetAsyncKeyState(button.showTeammatesBtn) & 1)
    settings.showTeammates = !settings.showTeammates;
  if (GetAsyncKeyState(button.snaplinesBtn) & 1)
    settings.snaplines = !settings.snaplines;
  if (GetAsyncKeyState(button.box2DBtn) & 1) settings.box2D = !settings.box2D;
  if (GetAsyncKeyState(button.status2DBtn) & 1)
    settings.status2D = !settings.status2D;
  if (GetAsyncKeyState(button.statusTextBtn) & 1)
    settings.statusText = !settings.statusText;
  if (GetAsyncKeyState(button.box3DBtn) & 1) settings.box3D = !settings.box3D;
  if (GetAsyncKeyState(button.velEspBtn) & 1)
    settings.velEsp = !settings.velEsp;
  if (GetAsyncKeyState(button.headlineEspBtn) & 1)
    settings.headlineEsp = !settings.headlineEsp;
  if (GetAsyncKeyState(button.rcsCrosshairBtn) & 1)
    settings.rcsCrosshair = !settings.rcsCrosshair;
  if (GetAsyncKeyState(button.rcsControlBtn) & 1)
    settings.rcsControl = !settings.rcsControl;
  if (GetAsyncKeyState(button.aimbotBtn) & 1)
    settings.aimbot = !settings.aimbot;
}