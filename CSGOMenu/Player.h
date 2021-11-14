#include <cstdint>

#include <Windows.h>

#include "Offsets.h"
#include "Vector3.h"

#ifndef PLAYER_H
#define PLAYER_H

class Vector3;
class Player {
private:
	Player();
public:
	static int* GetMaxPlayer();
	static Player* GetPlayer(int index);

	int* GetHealth();
	int* GetTeam(); 
	Vector3* GetOrigin();
	Vector3* GetViewOffset();
	Vector3* GetBonePos(int boneId);
};

#endif