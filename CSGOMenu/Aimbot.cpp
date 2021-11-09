#include "Aimbot.h"

Player* GetClosestEnemy() {
  LocalPlayer* localPlayer = LocalPlayer::Get();

  float closestDistance = std::numeric_limits<float>::max();
  int index = -1;

  Beep(100, 400);
  for (int i = 0; i < *Player::GetMaxPlayer(); i++) {
    Beep(200, 400);
    Player* currentPlayer = Player::GetPlayer(i);
    Beep(300, 400);

    if (!currentPlayer || !*(uint32_t*)currentPlayer == (uint32_t)localPlayer) {
      Beep(400, 400);

      continue;
    }
    Beep(500, 400);
    if (*currentPlayer->GetTeam() < 1) {
      Beep(600, 400);

      continue;
    }
    Beep(700, 400);
    if ((*localPlayer->GetHealth() < 1)) {
      Beep(800, 400);

      continue;
    }
    if (*currentPlayer->GetTeam() < 1 || *localPlayer->GetHealth() < 1) {
      Beep(900, 400);

      continue;
    }

    Beep(1000, 400);

    float currentDistance =
        localPlayer->GetDistance(currentPlayer->GetOrigin());

    Beep(1100, 400);
    if (currentDistance < closestDistance) {
      closestDistance = currentDistance;
      index = i;
    }
    Beep(1200, 400);
  }

  if (closestDistance == -1) return NULL;

  return Player::GetPlayer(index);
}

void Run() {
  try {
    Beep(1000, 400);
    Player* target = GetClosestEnemy();
    Beep(2000, 400);
    if (target) {
      Beep(3000, 400);
      math::Vector3 tar = target->GetBonePos(8);
      Beep(4000, 400);

      std::wstringstream ws;
      ws << "Target: X:" << tar.x << "\tY: " << tar.y << "\t Z:" << tar.z;
      MessageBox(NULL, ws.str().c_str(), L"Hallo du Nuttöööööööö",
                 MB_OK | MB_SETFOREGROUND);

      LocalPlayer::Get()->AimAt(tar);
      Beep(5000, 400);
    }
  } catch (std::exception& ex) {
    ex;
    Beep(6000, 10000);
  }
}