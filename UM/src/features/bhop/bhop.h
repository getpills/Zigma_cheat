#pragma once
#include "../src/include.h"
#include "../src/structs/vector.h"
#include "../src/offsets/client_dll.hpp"
#include "../src/offsets/offsets.hpp"
#include "../src/back/UMCommunication.h"
#include "../src/back/ezAttach.h"
#include "../../settings/settings.h"

using namespace client_dll;
using namespace C_BaseEntity;
using namespace CCSPlayerController;
using namespace C_BasePlayerPawn;
using namespace C_CSPlayerPawnBase;
using namespace C_BaseModelEntity;
using namespace EntitySpottedState_t;

void bhop()
{

	uintptr_t pawnAddress = driver::read_memory<uintptr_t>(driv, client + dwLocalPlayerPawn);

	if (pawnAddress == 0)
		return;

	uint32_t flags = driver::read_memory<uint32_t>(driv, pawnAddress + m_fFlags);
	bool in_air = flags & (1 << 0);
	bool space_pressed = GetAsyncKeyState(VK_XBUTTON2);
	
	if (space_pressed && in_air) {
		
		
		keybd_event(VK_SPACE, 0, 0, 0);
		
		keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
		Sleep(30);
	}
	// 65664 - in air
	// 65665 - on ground
	//cout << flags << " || " << (int)in_air << " || " << (int)space_pressed << endl;


}