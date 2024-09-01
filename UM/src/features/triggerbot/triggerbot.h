#pragma once
#include "../src/include.h"
#include "../src/structs/vector.h"
#include "../src/offsets/client_dll.hpp"
#include "../src/offsets/offsets.hpp"
#include "../src/back/UMCommunication.h"
#include "../src/back/ezAttach.h"

using namespace client_dll;
using namespace C_BaseEntity;
using namespace CCSPlayerController;
using namespace C_BasePlayerPawn;
using namespace C_CSPlayerPawnBase;

inline int TriggerDelay1 = settings::TriggerDelay;
inline int ShotDuration1 = settings::ShotDuration;


inline std::chrono::time_point<std::chrono::system_clock> timepoint1 = std::chrono::system_clock::now();
inline std::chrono::time_point<std::chrono::system_clock> startTime1 = std::chrono::system_clock::now();
inline bool recorded = false;

void ReleaseMouseButton1()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ShotDuration1));
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void triggerbot()
{
	pawn::Local localPlayer;

	

	localPlayer.pawnAddress = driver::read_memory<uintptr_t>(driv, client + client_dll::dwLocalPlayerPawn);
	DWORD crosshair = driver::read_memory<DWORD>(driv, localPlayer.pawnAddress + m_iIDEntIndex);
	if (crosshair == -1)
		return;
	
	localPlayer.team = driver::read_memory<int>(driv, localPlayer.pawnAddress + C_BaseEntity::m_iTeamNum);

	uintptr_t entityList = driver::read_memory<uintptr_t>(driv, client + client_dll::dwEntityList);

	uintptr_t listEntry = driver::read_memory<uintptr_t>(driv, entityList + 0x8 * (crosshair >> 9) + 0x10);

	uintptr_t entity = driver::read_memory<uintptr_t>(driv, listEntry + 120 * (crosshair & 0x1ff));
	
	if (!entity)
		return;

	if (localPlayer.team == driver::read_memory<uint8_t>(driv, entity + C_BaseEntity::m_iTeamNum))
		return;
	
	
	static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
	auto CurTimePoint = std::chrono::steady_clock::now();

	if (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(TriggerDelay1))
	{

		const bool isAlreadyShooting = GetAsyncKeyState(VK_LBUTTON) < 0;
		if (!isAlreadyShooting)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			std::thread TriggerThread(ReleaseMouseButton1);
			TriggerThread.detach();
		}
		LastTimePoint = CurTimePoint;

	}

	/*
	if (crosshair > 0)
	{

	
	if (GetAsyncKeyState(VK_XBUTTON1))
	{

		
		

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

			Sleep(5);
		

		
	}
	
	Sleep(1);

	}

	*/
	

}

