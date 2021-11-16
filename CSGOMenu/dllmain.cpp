#include <iostream>
#include <sstream>
#include <string.h>

#include "includes.h"
#include "extern.h"
#include "Hook/drawing.h"
#include "Aimbot/Aimbot.h"


void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
//Esp::ESP* esp;

#pragma region DrawEsp
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice) pDevice = o_pDevice;

	//Menu
	int menuOffX = windowWidth / 2;
	int menuOffY = 70;
	D3DCOLOR enabled = D3DCOLOR_ARGB(255, 0, 255, 0);
	D3DCOLOR disabled = D3DCOLOR_ARGB(255, 255, 0, 0);

	if (!esp->settings.showMenu) {
		DrawText("Show Menu [INSERT]", menuOffX, menuOffY, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else {
		DrawText("Shot Teamates[F1]", menuOffX, menuOffY + 0 * 12, esp->settings.showTeammates ? enabled : disabled);
		DrawText("Snaplines [F2]", menuOffX, menuOffY + 1 * 12, esp->settings.snaplines ? enabled : disabled);
		DrawText("2D Box [F3]", menuOffX, menuOffY + 2 * 12, esp->settings.box2D ? enabled : disabled);
		DrawText("2D Statusbars [F4]", menuOffX, menuOffY + 3 * 12, esp->settings.status2D ? enabled : disabled);
		DrawText("Statustext [F5]", menuOffX, menuOffY + 4 * 12, esp->settings.statusText ? enabled : disabled);
		DrawText("3D Box [F6]", menuOffX, menuOffY + 5 * 12, esp->settings.box3D ? enabled : disabled);
		DrawText("Velocity ESP [F7]", menuOffX, menuOffY + 6 * 12, esp->settings.velEsp ? enabled : disabled);
		DrawText("Headline ESP [F8]", menuOffX, menuOffY + 7 * 12, esp->settings.headlineEsp ? enabled : disabled);
		DrawText("Recoil Crosshair [F9]", menuOffX, menuOffY + 8 * 12, esp->settings.rcsCrosshair ? enabled : disabled);
		DrawText("Hide Menu [INSERT]", menuOffX, menuOffY + 9 * 12, esp->settings.showMenu ? enabled : disabled);
	}


	//Loop through EntityList and Draw
	for (int i = 1; i < 32; i++) {

		//Get Player
		Entity::Ent* currentPlayer = esp->entList->ents[i].ent;

		//Check if Player Dead, or nullptr
		if (!esp->CheckValidEnt(esp->entList->ents[i].ent)) continue;


		//Check Players Team, and set Line Color
		D3DCOLOR color;
		if (currentPlayer->iTeamNum == esp->localEnt->iTeamNum)
			color = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			color = D3DCOLOR_ARGB(255, 255, 0, 0);

		if (!esp->settings.showTeammates && (currentPlayer->iTeamNum == esp->localEnt->iTeamNum)) continue;

		//Define Drawing Position

		Vec3 entHead3D = esp->GetBonePos(currentPlayer, 8);
		entHead3D.z += 8;
		Vec2 entPos2D, entHeadPos2D;

		//WorldToScreen:
		//Checks if Position is valid, and turns it into a 2D Position on Player Screen, at address of parameter 2
		if (esp->WorldToScreen(currentPlayer->vecOrigin, entPos2D)) {

			//VelESP
			if (esp->settings.velEsp) {
				Vec3 velOff = currentPlayer->vecOrigin + (currentPlayer->vecVelocity * 0.25f);
				Vec2 velOff2D;

				if (esp->WorldToScreen(velOff, velOff2D)) {
					DrawLine(entPos2D, velOff2D, 1, color);
					DrawFilledRect(velOff2D.x - 1, velOff2D.y - 1, 2, 2, color);
				}
			}

			//Line to Player
			if(esp->settings.snaplines)
			DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 1, color);
			
			if (esp->WorldToScreen(entHead3D, entHeadPos2D)) {
				if(esp->settings.box2D)
				DrawEspBox2D(entPos2D, entHeadPos2D, 1, color); 

				if (esp->settings.box3D)
					DrawEspBox3D(entHead3D, currentPlayer->vecOrigin, currentPlayer->angEyeAnglesY, 25, 1, color);

				if (esp->settings.status2D) {
					// Draw Health and Armor Value
					int height = ABS(entPos2D.y - entHeadPos2D.y);
					int dx = (entPos2D.x - entHeadPos2D.x);

					float healthPerc = currentPlayer->iHealth / 100.f;
					float armorPerc = currentPlayer->armorValue / 100.f;

					Vec2 botHealth, topHealth, botArmor, topArmor;
					int healthHeight = height * healthPerc;
					int armorHeight = height * armorPerc;

					botHealth.y = botArmor.y = entPos2D.y;

					botHealth.x = entPos2D.x - (height / 4) - 2;
					botArmor.x = entPos2D.x + (height / 4) + 2;

					topHealth.y = entHeadPos2D.y + height - healthHeight;
					topArmor.y = entHeadPos2D.y + height - armorHeight;

					topHealth.x = entPos2D.x - (height / 4) - 2 - (dx * healthPerc);
					topArmor.x = entPos2D.x + (height / 4) + 2 - (dx * armorPerc);

					DrawLine(botHealth, topHealth, 2, D3DCOLOR_ARGB(255, 46, 139, 87));
					DrawLine(botArmor, topArmor, 2, D3DCOLOR_ARGB(255, 30, 144, 255));
				}

				if (esp->settings.headlineEsp) {
					Vec3 head3D = esp->GetBonePos(currentPlayer, 8);
					Vec3 entAngles;
					entAngles.x = currentPlayer->angEyeAnglesX;
					entAngles.y = currentPlayer->angEyeAnglesY;
					entAngles.z = 0;

					Vec3 endPoint = esp->TransformVec(head3D, entAngles, 60);
					Vec2 endPoint_2D, head_2D;
					esp->WorldToScreen(head3D, head_2D);
					if (esp->WorldToScreen(endPoint, endPoint_2D)) {
						DrawLine(head_2D, endPoint_2D, 1, color);
					}
				}

				if (esp->settings.statusText) {
					std::stringstream s1, s2;
					s1 << currentPlayer->iHealth;
					s2 << currentPlayer->armorValue;

					std::string t1 = "HP: " + s1.str();
					std::string t2 = "AP: " + s2.str();
					char* healthMsg = (char*)t1.c_str();
					char* armorMsg = (char*)t2.c_str();

					DrawText(healthMsg, entPos2D.x, entPos2D.y, D3DCOLOR_ARGB(255, 255, 255, 255));
					DrawText(armorMsg, entPos2D.x, entPos2D.y + 12, D3DCOLOR_ARGB(255, 255, 255, 255));

					if (!currentPlayer->bHasHelmet)
						DrawText("No Helmet", entPos2D.x, entPos2D.y + 24, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

			}
		}

	}

	if (esp->settings.rcsCrosshair) {
		//Draw Recoil Crosshair
		Vec2 l, r, t, b;

		l = r = t = b = esp->crosshair2D;
		l.x -= esp->crosshairSize;
		r.x += esp->crosshairSize;
		b.y += esp->crosshairSize;
		t.y -= esp->crosshairSize;

		DrawLine(l, r, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		DrawLine(t, b, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//Draws Frame
	oEndScene(pDevice);
}
#pragma endregion



DWORD WINAPI OnDllAttach(HMODULE hModule) {

//#ifdef _DEBUG
//	AllocConsole();
//	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
//	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
//	SetConsoleTitle(L"Aimbot - DEBUG");
//#endif // _DEBUG

	//DirectX Hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	//ESP
	esp = new Esp::ESP();
	esp->Init();
	//Aimbot
	LocalPlayer::Get();

	//Routine 
	while (!(GetAsyncKeyState(VK_END))){ //Press END to Exit and Detach
		//ESP
		esp->Update();

		Vec3 pAng = esp->localEnt->aimPunchAngle;
		esp->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
		esp->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);

		//Aimbot
		if(GetAsyncKeyState(VK_MENU))
		Run();
	}

	//unhook DirectX
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	//Prevent Crashing
	Sleep(1000);

	//Detach DLL
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved) {
	
	//Checks For Call Reason
	if (reason == DLL_PROCESS_ATTACH) {
		//Starts MainThread
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, 0, 0);
	}
	else if (reason == DLL_PROCESS_DETACH) {
//#ifdef _DEBUG
//		fclose((FILE*)stdin);
//		fclose((FILE*)stdout);
//
//		HWND hw_ConsoleHwnd = GetConsoleWindow();
//		FreeConsole();
//		PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0);
//#endif // _DEBUG
	}

return TRUE;
}

