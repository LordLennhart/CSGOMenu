#include <Windows.h>

#include <cstdint>

#include "Offsets.h"
#include "Math.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	Player();
public:
	static int* GetMaxPlayer();
	static Player* GetPlayer(int index);

	int* GetHealth(); 
	int* GetTeam();
	math::Vector3* GetOrigin();
	math::Vector3* GetViewOffset();
	math::Vector3 GetBonePOs(int boneId);
};

#endif // !PLAYER_H
