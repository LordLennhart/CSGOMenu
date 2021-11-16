#include "../offsets.h"
#include "../Math/Vector.h"
#include "../includes.h"
//#include "Players.h"

#ifndef ESP_H
#define ESP_H

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) \
  struct {                                  \
    unsigned char MAKE_PAD(offset);         \
    type name;                              \
  }

#define ABS(x) ((x < 0) ? (-x) : (x))
#define TORAD(x) ((x)*0.01745329252)
#define W2S(x, y) esp->WorldToScreen(x, y)

namespace Entity {

// Get Player Values
struct Ent {
  union {
    // isDormant
    DEFINE_MEMBER_N(bool, isDormant, 0xED);
    // iHealth
    DEFINE_MEMBER_N(int, iHealth, 0x100);
    // vecOrigin
    DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);
    // iTeamNum
    DEFINE_MEMBER_N(int, iTeamNum, 0xF4);
    // BoneMatrix
    DEFINE_MEMBER_N(int, boneMatrix, offsets::m_dwBoneMatrix);
    // armorValue
    DEFINE_MEMBER_N(int, armorValue, offsets::m_ArmorValue);
    // aimPunchAngle
    DEFINE_MEMBER_N(Vec3, aimPunchAngle, offsets::m_aimPunchAngle);
    // angEyeAngles X
    DEFINE_MEMBER_N(float, angEyeAnglesX, offsets::m_angEyeAnglesX);
    // angEyeAngles Y
    DEFINE_MEMBER_N(float, angEyeAnglesY, offsets::m_angEyeAnglesY);
    // vecVelocity
    DEFINE_MEMBER_N(Vec3, vecVelocity, offsets::m_vecVelocity);
    // bHasHelmet
    DEFINE_MEMBER_N(bool, bHasHelmet, offsets::m_bHasHelmet);
  };
};

struct EntListObj {
  Ent* ent;
  char padding[12];
};

// Player List with, values
struct EntList {
  EntListObj ents[32];
};

}  // namespace Entity

namespace Esp {

class ESP {
 public:
  uintptr_t engine;
  uintptr_t client;
  Entity::Ent* localEnt;
  Entity::EntList* entList;
  float viewMatrix[16];

  ID3DXLine* LineL;
  ID3DXFont* FontF;

  Vec2 crosshair2D;
  int crosshairSize = 4;

  ~ESP();

  void Init();
  void Update();

  void CheckButtons();

  bool CheckValidEnt(Entity::Ent* ent);
  bool WorldToScreen(Vec3 pos, Vec2& screen);
  Vec3 GetBonePos(Entity::Ent* ent, int boneId);
  Vec3 TransformVec(Vec3 src, Vec3 angl, float distance);

  struct Settings {
    bool showMenu = false;
    bool showTeammates = true;
    bool snaplines = true;
    bool box2D = false;
    bool status2D = false;
    bool statusText = true;
    bool box3D = true;
    bool velEsp = false;
    bool headlineEsp = false;
    bool rcsCrosshair = true;
    bool rcsControl = true;
    bool aimbot = true;
  } settings;

  struct Buttons {
    DWORD showMenuBtn = VK_INSERT;
    DWORD showTeammatesBtn = VK_F1;
    DWORD snaplinesBtn = VK_F2;
    DWORD box2DBtn = VK_F3;
    DWORD status2DBtn = VK_F4;
    DWORD statusTextBtn = VK_F5;
    DWORD box3DBtn = VK_F6;
    DWORD velEspBtn = VK_F7;
    DWORD headlineEspBtn = VK_F8;
    DWORD rcsCrosshairBtn = VK_F9;
    DWORD rcsControlBtn = VK_F10;
    DWORD aimbotBtn = VK_F11;
  } button;
};

}  // namespace Esp
#endif  // !ESP_H