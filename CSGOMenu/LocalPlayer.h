#include <Windows.h>

#include <cstdint>

#include "Math.h"
#include "Offsets.h"

#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

class LocalPlayer {
private:

	LocalPlayer();

public:

	static LocalPlayer* Get();

	math::Vector3* GetOrigin();
	math::Vector3* GetViewOffset();
	int* GetHealth();
	int* GetTeam();
	void AimAt(math::Vector3* target);
	float GetDistance(math::Vector3*);
};

#endif // !LOCALPLAYER_H