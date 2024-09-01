#pragma once

#include "../src/include.h"
#include "../src/offsets/client_dll.hpp"
#include "../src/offsets/offsets.hpp"
#include "../src/back/UMCommunication.h"
#include "../src/back/ezAttach.h"
#include "../src/structs/vector.h"
#include "../wh/draw.h"

using namespace client_dll;
using namespace C_BaseEntity;
using namespace CCSPlayerController;
using namespace C_BasePlayerPawn;
using namespace CGameSceneNode;
using namespace CPlayer_ObserverServices;




void Wall_Hack(int mode)
{
    pawn::Entity entity;
    pawn::Local localPlayer;
    vector <pawn::Entity> answ;

    uintptr_t entityList = driver::read_memory<uintptr_t>(driv, client + client_dll::dwEntityList);
    uintptr_t listEntry = driver::read_memory<uintptr_t>(driv, entityList + 0x10);

    localPlayer.pawnAddress = driver::read_memory<uintptr_t>(driv, client + client_dll::dwLocalPlayerPawn);
    localPlayer.team = driver::read_memory<int>(driv, localPlayer.pawnAddress + C_BaseEntity::m_iTeamNum);
    localPlayer.viewMatrix = driver::read_memory<view_matrix_t>(driv, client + client_dll::dwViewMatrix);

    for (int i = 1; i < 64; i++)
    {
        uintptr_t currentController = driver::read_memory<uintptr_t>(driv, listEntry + i * 0x78);
        if (currentController == NULL)
            continue;

        int pawnHandle = driver::read_memory<int>(driv, currentController + m_hPlayerPawn);
        if (pawnHandle == 0)
            continue;

        uintptr_t listEntry2 = driver::read_memory<uintptr_t>(driv, entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);
        if (listEntry2 == NULL)
            continue;

        uintptr_t currentPawn = driver::read_memory<uintptr_t>(driv, listEntry2 + 0x78 * (pawnHandle & 0x1FF));
        if (currentPawn == localPlayer.pawnAddress)
            continue;

        int lifeState = driver::read_memory<int>(driv, currentPawn + m_lifeState);
        if (lifeState != 256) continue;

        


        Draw::DrawWallHack(currentPawn, { 255,255,255 }, localPlayer, mode, currentController);
    }
    

   

    return;
}