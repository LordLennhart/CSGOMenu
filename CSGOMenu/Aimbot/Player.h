#include <cstdint>
#include <iostream>

#include <Windows.h>

#include "../offsets.h"
#include "../Math/Vector3.h"

#ifndef PLAYER_H
#define PLAYER_H

class Vector3;
class Player {
private:
	Player();
public:
	static int playerIndex;
	static int* GetMaxPlayer();
	static Player* GetPlayer(int index);

	int* GetHealth();
	int* GetTeam(); 
	bool SeenBy();
	//bool SeenBy(Player* player);
	Vector3* GetOrigin();
	Vector3* GetViewOffset();
	Vector3* GetBonePos(int boneId);
};

#endif