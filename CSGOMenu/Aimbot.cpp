#include "Aimbot.h"

Player* GetClosestEnemy() {
  LocalPlayer* localPlayer = LocalPlayer::Get();

  float closestDistance = 999999;
  int index = -1;

  for (int i = 0; i < *Player::GetMaxPlayer(); i++) {
    Player* currentPlayer = Player::GetPlayer(i);

    if (!currentPlayer || !*(uint32_t*)currentPlayer == (uint32_t)localPlayer)
      continue;
    if (*currentPlayer->GetTeam() < 1 || *localPlayer->GetHealth() < 1)
      continue;
    if (*currentPlayer->GetTeam() < 1 || *localPlayer->GetHealth() < 1)
      continue;

    float currentDistance =
        localPlayer->GetDistance(currentPlayer->GetOrigin());
    if (currentDistance < closestDistance) {
      closestDistance = currentDistance;
      index = i;
    }
  }

  if (closestDistance == -1) return NULL;

  return Player::GetPlayer(index);
}

void Run() { 
    abort();
    Player* target = GetClosestEnemy();
    
    if (target) LocalPlayer::Get()->AimAt(target->GetBonePos(8));
}