
#pragma once
#include "../src/include.h"
#include "../src/structs/vector.h"
#include "../src/offsets/client_dll.hpp"
#include "../src/offsets/offsets.hpp"
#include "../src/back/UMCommunication.h"
#include "../src/back/ezAttach.h"
#include "../../settings/settings.h"
#include "../wh/draw.h"

using namespace client_dll;
using namespace C_BaseEntity;
using namespace CCSPlayerController;
using namespace C_BasePlayerPawn;
using namespace C_CSPlayerPawnBase;
using namespace C_BaseModelEntity;
using namespace EntitySpottedState_t;


extern int ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
extern int ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;

Vector3 RCSScale = { 0.5f,0.5f };
int RCSBullet = 1;







/*
void UpdateAngles(Vector3& Angles)
{

	uintptr_t localPlayer = driver::read_memory<uintptr_t>(driv, client + dwLocalPlayerPawn);

	auto oldPunch = Vector3{ };
	auto shotsFired = driver::read_memory<int32_t>(driv, localPlayer + m_iShotsFired);

	Vector3 localViewAngles = driver::read_memory<Vector3>(driv, client + client_dll::dwViewAngles);
	Vector3 aimPunch1 = driver::read_memory<Vector3>(driv, localPlayer + C_CSPlayerPawn::m_aimPunchAngle);
	
	C_UTL_VECTOR AimPunchCache = driver::read_memory<C_UTL_VECTOR>(driv, localPlayer + C_CSPlayerPawn::m_aimPunchCache);

	if (shotsFired)
	{
		uintptr_t clientState;
		auto viewAngles = localViewAngles;
		auto aimPunch = aimPunch1;

		auto newAngles = Vector3
		{
			viewAngles.x + oldPunch.x - aimPunch.x * 2.f,
			viewAngles.y + oldPunch.y - aimPunch.y * 2.f,
		};

		if (newAngles.x > 89.f)
			newAngles.x = 89.f;

		if (newAngles.x < -89.f)
			newAngles.x = -89.f;

		while (newAngles.y > 180.f)
			newAngles.y -= 360.f;

		while (newAngles.y < -180.f)
			newAngles.y += 360.f;

		newAngles.x += ScreenCenterX;
		newAngles.y += ScreenCenterY;
		Angles = newAngles;
		oldPunch = aimPunch;
	}
	else
	{
		oldPunch.x = oldPunch.y = 0.f;
	}
	
	
	
}*/


 void RecoilControl()
{

	 static Vector3 OldPunch;
	
	 uintptr_t localPlayer = driver::read_memory<uintptr_t>(driv, client + dwLocalPlayerPawn);
	 int32_t shotsFired = driver::read_memory<int32_t>(driv, localPlayer + client_dll::C_CSPlayerPawn::m_iShotsFired);
	 Vector3 localViewAngles = driver::read_memory<Vector3>(driv, client + client_dll::dwViewAngles);
	 Vector3 aimPunch = driver::read_memory<Vector3>(driv, localPlayer + C_CSPlayerPawn::m_aimPunchAngle);


	 uintptr_t Sensitivity = driver::read_memory<uintptr_t>(driv, client + client_dll::dwSensitivity);
	 float flSensitivity = driver::read_memory<float>(driv, Sensitivity + 0x40);
	
	

	
	if (shotsFired > RCSBullet)
	{
		Vector3 viewAngles = localViewAngles;
		Vector3 delta = viewAngles - (viewAngles + (OldPunch - (aimPunch * 2.f)));
		//cout << delta.x << " || " << delta.y << endl;
		int MouseX = (int)(delta.y / (flSensitivity * 0.011f) * RCSScale.x);
		int MouseY = (int)(delta.x / (flSensitivity * 0.011f) * RCSScale.y);

		
		

		

		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			mouse_event(MOUSEEVENTF_MOVE, MouseX, -MouseY, 0, 0);

		OldPunch = aimPunch * 2.0f;

		
	}
	else
	{
		OldPunch = Vector3{ 0,0 };
	}

	
}


void aimbot() {


	


	uintptr_t enityList = driver::read_memory<uintptr_t>(driv, client + dwEntityList);

	uintptr_t listEntry = driver::read_memory<uintptr_t>(driv, enityList + 0x10);

	uintptr_t pawnAddress = driver::read_memory<uintptr_t>(driv, client + dwLocalPlayerPawn);

	uint8_t Localteam = driver::read_memory<uint8_t>(driv, pawnAddress + m_iTeamNum);

	Vector3 Localorigin = driver::read_memory<Vector3>(driv, pawnAddress + m_vOldOrigin);

	Vector3 Localview = driver::read_memory<Vector3>(driv, pawnAddress + m_vecViewOffset);
	view_matrix_t viewMatrix = driver::read_memory<view_matrix_t>(driv, client + client_dll::dwViewMatrix);



	Vector3 localViewAngles = driver::read_memory<Vector3>(driv, client + client_dll::dwViewAngles);

	Vector3 localEyePos = driver::read_memory<Vector3>(driv, pawnAddress + C_BasePlayerPawn::m_vOldOrigin) +
		driver::read_memory<Vector3>(driv, pawnAddress + C_BaseModelEntity::m_vecViewOffset);




	float TargetX = 0.f;
	float TargetY = 0.f;
	//int MouseX = 0;
	//int MouseY = 0;

	

	Vector3 bestTarget;
	double distance = 100000000; 

	
	Draw::drawCircle(960, 540, settings::fov, ImColor(255, 255, 255, 255), 1);

	for (int i = 0; i < 64; i++)
	{

		if (listEntry == 0)
			continue;

		uintptr_t currentConroller = driver::read_memory<uintptr_t>(driv, listEntry + i * 0x78);
		if (currentConroller == 0)
			continue;

		int pawnHandle = driver::read_memory<int>(driv, currentConroller + m_hPlayerPawn);
		if (pawnHandle == 0)
			continue;

		uintptr_t listEntry2 = driver::read_memory<uintptr_t>(driv, enityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

		uintptr_t currentPawn = driver::read_memory<uintptr_t>(driv, listEntry2 + 0x78 * (pawnHandle & 0x1FF));
		if (currentPawn == 0)
			continue;

		uint32_t health = driver::read_memory<uint32_t>(driv, currentPawn + m_iHealth);
		uint8_t team = driver::read_memory<uint8_t>(driv, currentPawn + m_iTeamNum);
		int lifeState = driver::read_memory<int>(driv, currentPawn + m_lifeState);

		if (lifeState != 256)
			continue;

		if (team == Localteam)
			continue;

		//visible check
		bool spottedState = driver::read_memory<bool>(driv, currentPawn + (C_CSPlayerPawn::m_entitySpottedState + m_bSpotted));
		
		
		//visible check

		Vector3 origin = driver::read_memory<Vector3>(driv, currentPawn + m_vOldOrigin);
		Vector3 view = driver::read_memory<Vector3>(driv, currentPawn + m_vecViewOffset);

		uintptr_t sceneNode = driver::read_memory<uintptr_t>(driv, currentPawn + C_BaseEntity::m_pGameSceneNode);


		uintptr_t boneMatrix = driver::read_memory<uintptr_t>(driv, sceneNode + CSkeletonInstance::m_modelState + 0x80);
		Vector3 entityHead = driver::read_memory<Vector3>(driv, boneMatrix + 6 * 32);

		

		
		if (settings::visibleCheck == 1) {
			if (!spottedState) {
				continue;
			}

		}
		
		

		Vector3 ScreenPos = entityHead.WTS(viewMatrix);

		if(sqrt((ScreenPos.x - 960) * (ScreenPos.x - 960) + (ScreenPos.y - 540) * (ScreenPos.y - 540)) < distance)
		{ 
		
		distance = sqrt((ScreenPos.x - 960) * (ScreenPos.x - 960) + (ScreenPos.y - 540) * (ScreenPos.y - 540));
		bestTarget = ScreenPos;

		}
		

		
		

		

	}//for

	




	
	Vector3 ScreenPos = bestTarget;

	

	if (distance > settings::fov)
		return;

	
	
	if (ScreenPos.x != ScreenCenterX)
	{
		TargetX = (ScreenPos.x > ScreenCenterX) ? -(ScreenCenterX - ScreenPos.x) : ScreenPos.x - ScreenCenterX;
		TargetX /= settings::Smooth != 0.0f ? settings::Smooth : settings::fovSpedd;
		//TargetX = (TargetX + ScreenCenterX > ScreenCenterX * 2 || TargetX + ScreenCenterX < 0) ? 0 : TargetX;
	}

	if (ScreenPos.y != 0)
	{
		if (ScreenPos.y != ScreenCenterY)
		{
			TargetY = (ScreenPos.y > ScreenCenterY) ? -(ScreenCenterY - ScreenPos.y) : ScreenPos.y - ScreenCenterY;
			TargetY /= settings::Smooth != 0.0f ? settings::Smooth : settings::fovSpedd;
			//TargetY = (TargetY + ScreenCenterY > ScreenCenterY * 2 || TargetY + ScreenCenterY < 0) ? 0 : TargetY;
		}
	}

	

	


	
	if (GetAsyncKeyState(VK_XBUTTON1)) {
		if (settings::AimRCS == 1)
		{ 
		static Vector3 OldPunch;

		uintptr_t localPlayer = driver::read_memory<uintptr_t>(driv, client + dwLocalPlayerPawn);
		int32_t shotsFired = driver::read_memory<int32_t>(driv, localPlayer + client_dll::C_CSPlayerPawn::m_iShotsFired);
		//Vector3 localViewAngles = driver::read_memory<Vector3>(driv, client + client_dll::dwViewAngles);
		Vector3 aimPunch = driver::read_memory<Vector3>(driv, localPlayer + C_CSPlayerPawn::m_aimPunchAngle);


		uintptr_t Sensitivity = driver::read_memory<uintptr_t>(driv, client + client_dll::dwSensitivity);
		float flSensitivity = driver::read_memory<float>(driv, Sensitivity + 0x40);


		int MouseX = 0;
		int MouseY = 0;

		if (shotsFired > RCSBullet)
		{
			Vector3 viewAngles = localViewAngles;
			Vector3 delta = viewAngles - (viewAngles + (OldPunch - (aimPunch * 2.f)));
			//cout << delta.x << " || " << delta.y << endl;
			MouseX = (int)(delta.y / (flSensitivity * 0.011f) * RCSScale.x);
			MouseY = (int)(delta.x / (flSensitivity * 0.011f) * RCSScale.y);
			//cout << MouseX << " || " << MouseY << endl;



			MouseX += TargetX;
			MouseY += TargetY;

			

			
			
				mouse_event(MOUSEEVENTF_MOVE, MouseX, -MouseY, 0, 0);
			
			OldPunch = aimPunch * 2.0f;


		}
		else
		{
			OldPunch = Vector3{ 0,0 };
		}

		}// aim rcs

		mouse_event(MOUSEEVENTF_MOVE, TargetX, TargetY, NULL, NULL);

		
		
	}
	
	
}