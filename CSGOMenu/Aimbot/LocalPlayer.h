#include <cstdint>

#include <Windows.h>
#include <math.h>
#include <iostream>

#include "../Math/Vector3.h"
#include "../offsets.h"
#include "../Math/Vector.h"
#include "../extern.h"

#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

class Vector3;
class LocalPlayer {
private:
	LocalPlayer();
public:
	static LocalPlayer* Get();

	Vector3* GetOrigin();
	Vector3* GetViewOffset();
	int* GetHealth();
	int* GetShots();
	int* GetTeam();
	void AimAt(Vector3* target);
	float GetDistance(Vector3* other);

};

#endif // !LOCALPLAYER_H
