#include <Windows.h>
#include <math.h>

#include <cstdint>

#include "Math.h"
#include "Offsets.h"

#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#define PI 3.14159265359

class LocalPlayer {
private:

	LocalPlayer();

public:

	static LocalPlayer* Get();

	math::Vector3* GetOrigin();
	math::Vector3* GetViewOffset();
	int* GetHealth();
	int* GetTeam();
	void AimAt(math::Vector3 target);
	float GetDistance(math::Vector3* other);
};

#endif // !LOCALPLAYER_H