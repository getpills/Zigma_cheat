#pragma once
#include "../src/include.h"
#include <../external/ImGui/imgui.h>
#include "../src/offsets/client_dll.hpp"
#include "../src/offsets/offsets.hpp"
#include "../src/back/UMCommunication.h"
#include "../src/back/ezAttach.h"
#include "../src/structs/vector.h"
#include "../../settings/settings.h"
typedef struct
{
	ImU32 R;
	ImU32 G;
	ImU32 B;
	
} RGB;

ImU32 Color(RGB color)
{
	return IM_COL32(color.R, color.G, color.B, 255);
}





namespace Draw
{
	using namespace client_dll;
	using namespace C_BaseEntity;
	using namespace CCSPlayerController;
	using namespace C_BasePlayerPawn;
	vector<pair<int, int>> bones = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{5,7},{7,8},{8,9},{9,10},{9,10},{10,11},{5,12},{12,13},{13,14},{14,15},{0,22},{22,23},{23,24},{0,25},{25,26},{26,27}};
	void DrawRect(int x, int y, int w, int h, ImColor color, int thickness)
	{
		//if (x > 1920 || x < 0 || x + w>1920 || x + w < 0 || y>1080 || y + h > 1080 || y < 0 || y + h < 0) return;
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
	}

	void DrawRectByDots(int x1, int y1, int x2, int y2, ImColor color, int thickness)
	{
		//if (x1 >= 1920 || x1 <= 0 || x2 >= 1920 || x2 <= 0 || y1>= 1080 || y2 >= 1080 || y1 <= 0 || y2 <= 0) return;
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), color, 0, 0, thickness);
	}
	void DrawRectFilled(int x, int y, int w, int h, ImColor color)
	{
		//if (x >= 1920 || x <= 0 || x + w>= 1920 || x + w <= 0 || y>= 1080 || y + h >= 1080 || y <= 0 || y + h <= 0) return;
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
	}
	void DrawRectFilledByDots(int x1, int y1, int x2, int y2, ImColor color,int alpha)
	{
		//if (x1 > 1920 || x1 < 0 || x2>1920 || x2 < 0 || y1>1080 || y2 > 1080 || y1 < 0 || y2 < 0) return;
		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(x1, y1), ImVec2(x2, y2), settings::C_EnemyFade1,
		settings::C_EnemyFade2, settings::C_EnemyFade3, settings::C_EnemyFade4);
	
	}
	
	void drawLine(int x1, int y1, int x2, int y2, ImColor color, int thickness) {
		if (x1 < 0 || x1>1920 || y1 < 0 || y1>1080) return;
		if (x2 < 0 || x2>1920 || y2 < 0 || y2>1080) return;
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
	}
	void drawCircle(int x, int y, int radius, ImColor color, int thickness) {
		//if (x < 0 || x>1920 || y < 0 || y>1080) return;
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, 32, 2.0f);
	}
	void drawText(int x, int y, string text) {

		const char* textCStr = text.c_str();

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1, y), IM_COL32_BLACK, textCStr);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - 1, y), IM_COL32_BLACK, textCStr);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y - 1), IM_COL32_BLACK, textCStr);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y + 1), IM_COL32_BLACK, textCStr);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), IM_COL32_WHITE, textCStr);
	}

	void drawRadar(uintptr_t currentPawn) {
		uintptr_t localPlayer = driver::read_memory<uintptr_t>(driv, client + client_dll::dwLocalPlayerPawn);
		Vector3 localViewAngles = driver::read_memory<Vector3>(driv, client + client_dll::dwViewAngles);
		Vector3 enemyPos = driver::read_memory<Vector3>(driv, currentPawn + C_BasePlayerPawn::m_vOldOrigin);
		Vector3 myPos = driver::read_memory<Vector3>(driv, localPlayer + C_BasePlayerPawn::m_vOldOrigin);
		drawCircle(settings::radar_center.first, settings::radar_center.second, 2, IM_COL32_WHITE, 2);

		Vector3 dist;
		dist.x = (enemyPos.x - myPos.x) * settings::radar_scale;
		dist.y = (enemyPos.y - myPos.y) * settings::radar_scale;

		float rho = sqrt((dist.x / 12.f) * (dist.x / 12.f) + (dist.y / 12.f) * (dist.y / 12.f));
		float phi = (localViewAngles.y / 180.f) * (3.1415926) + atan2(dist.x, dist.y);
		dist.x = rho * cosf(phi);
		dist.y = rho * sinf(phi);
		if (rho > settings::radar_radius) return;
		drawCircle(settings::radar_center.first - dist.x, settings::radar_center.second - dist.y, 5, IM_COL32_WHITE, 5);
	}


	void DrawWallHack(uintptr_t currentPawn, RGB color, pawn::Local localPlayer, int mode, uintptr_t currentController) {
		//check all bones

		//draw bones
		//draw box
		//draw nickname
		//draw weapon
		RGB white{ 255,255,255 };
		RGB Red{ 255, 0, 0 };

		int draw_bones = mode & 1;
		int draw_boxes = mode & 2;
		int draw_health = mode & 4;
		int draw_tracer = mode & 8;
		int draw_name = mode & 16;

		bool exists = 0;

		uint8_t enemyTeam = driver::read_memory<uintptr_t>(driv, currentPawn + m_iTeamNum);

		int top_dot=0, bottom_dot=0, left_dot=0, right_dot=0;
		float top_cord = 1080, bottom_cord = 0, left_cord = 1920, right_cord = 0;
		uintptr_t sceneNode = driver::read_memory<uintptr_t>(driv, currentPawn + C_BaseEntity::m_pGameSceneNode);
		uintptr_t boneMatrix = driver::read_memory<uintptr_t>(driv, sceneNode + CSkeletonInstance::m_modelState + 0x80);
		for (int i = 0; i < bones.size(); i++) {
			Vector3 dot1 = driver::read_memory<Vector3>(driv, boneMatrix + bones[i].first * 32);
			Vector3 dot2 = driver::read_memory<Vector3>(driv, boneMatrix + bones[i].second * 32);
			dot1 = dot1.WTS(localPlayer.viewMatrix);
			dot2 = dot2.WTS(localPlayer.viewMatrix);

			if (dot1.x > 0 && dot1.x < 1920 && dot1.y>0 && dot1.y < 1080) exists = 1;
			if (dot2.x > 0 && dot2.x < 1920 && dot2.y>0 && dot2.y < 1080) exists = 1;

			
			if (draw_bones && localPlayer.team != enemyTeam) drawLine(dot1.x, dot1.y, dot2.x, dot2.y, settings::C_EnemySkelet, 1.5);
			if (draw_bones && localPlayer.team == enemyTeam && settings::teamSkeleton == 1) drawLine(dot1.x, dot1.y, dot2.x, dot2.y, settings::C_TeamSkelet, 1.5);

			if (dot1.y < top_cord) {
				top_cord = dot1.y;
				top_dot = bones[i].first;
			}
			if (dot2.y < top_cord) {
				top_cord = dot2.y;
				top_dot = bones[i].second;
			}

			if (dot1.y > bottom_cord) {
				bottom_cord = dot1.y;
				bottom_dot = bones[i].first;
			}
			if (dot2.y > bottom_cord) {
				bottom_cord = dot2.y;
				bottom_dot = bones[i].second;
			}

			if (dot1.x < left_cord) {
				left_cord = dot1.x;
				left_dot = bones[i].first;
			}
			if (dot2.x < left_cord) {
				left_cord = dot2.x;
				left_dot = bones[i].second;
			}

			if (dot1.x > right_cord) {
				right_cord = dot1.x;
				right_dot = bones[i].first;
			}
			if (dot2.x > right_cord) {
				right_cord = dot2.x;
				right_dot = bones[i].second;
			}
		}

		//radar.
		if(settings::radar == 1)
		{ 
		drawRadar(currentPawn);
		}

		if (!exists) return;

		Vector3 bonehead = driver::read_memory<Vector3>(driv, boneMatrix + 6 * 32);
		bonehead.z += 2;
		Vector3 headOffset = bonehead;
		headOffset.z = headOffset.z + 5;
		bonehead = bonehead.WTS(localPlayer.viewMatrix);
		headOffset = headOffset.WTS(localPlayer.viewMatrix);
		int radius = sqrt((headOffset.x - bonehead.x) * (headOffset.x - bonehead.x) + (headOffset.y - bonehead.y) * (headOffset.y - bonehead.y));
		if(draw_bones && localPlayer.team != enemyTeam) drawCircle(bonehead.x, bonehead.y, radius, settings::C_EnemySkelet, 2);
		if (draw_bones && localPlayer.team == enemyTeam && settings::teamSkeleton == 1) drawCircle(bonehead.x, bonehead.y, radius, settings::C_TeamSkelet, 2);
		
		

		

		
		if (draw_boxes || draw_health || draw_tracer || draw_name) {
			//uint8_t enemyTeam = driver::read_memory<uintptr_t>(driv, currentPawn + m_iTeamNum);
			uint32_t health = driver::read_memory<uint32_t>(driv, currentPawn + m_iHealth);
			Vector3 origin = driver::read_memory<Vector3>(driv, currentPawn + m_vOldOrigin);
			Vector3 head = { origin.x, origin.y, origin.z + 75.f };

			Vector3 screenPos = origin.WTS(localPlayer.viewMatrix);
			Vector3 screenHead = head.WTS(localPlayer.viewMatrix);

			float height = screenPos.y - screenHead.y;
			float width = height / 2.4f;
			RGB enemy = { 255, 0, 0 };
			RGB temmates = { 0, 128, 0 };
			RGB CurrentColor;
			if (draw_boxes ) {
				/*if (enemyTeam != localPlayer.team)
				{
					CurrentColor = enemy;
					Draw::DrawRect(
						top_cord,
						left_cord,
						width,
						height,
						enemy,
						1.5
					);
				}

				if (enemyTeam == localPlayer.team)
				{
					CurrentColor = temmates;
					Draw::DrawRect(
						screenHead.x - width / 2,
						screenHead.y,
						width,
						height,
						temmates,
						1.5
					);
				}*/
				if (top_dot == 6) {
					top_cord = bonehead.y-radius;
				}
				
				if(localPlayer.team != enemyTeam)
					DrawRectByDots(left_cord, top_cord, right_cord, bottom_cord, settings::C_EnemyBox, 1.5);
				if (localPlayer.team == enemyTeam && settings::teamBox == 1)
					DrawRectByDots(left_cord, top_cord, right_cord, bottom_cord, settings::C_TeamBox, 1.5);


				if(enemyTeam != localPlayer.team && settings::fade1 == 1){
				DrawRectFilledByDots(left_cord + 1, top_cord + 1, right_cord - 1, bottom_cord + 1, { 128,0,128 }, 128);
				}
				
			}

			if (draw_health) {
				/*Vector3 original_screenHead = screenHead;
				original_screenHead.x = screenHead.x - (width / 2);
				float original_width = width / 10.f;

				float original_height = height;
				original_height = -1 * height * (health / 100.f);
				Draw::DrawRectFilled(original_screenHead.x, screenPos.y, original_width, original_height, { 50, 205 ,50 });
				Draw::DrawRect(original_screenHead.x, screenPos.y, original_width, original_height, { 0, 100 ,0 }, 1.5);*/
				
				int width = -1*(right_cord - left_cord);
				int height = (top_cord - bottom_cord)*(health /100.f);
				width = width / 10;
				Draw::DrawRectFilled(left_cord, bottom_cord, width, height, settings::C_EnemyHealth);
				Draw::DrawRect(left_cord, bottom_cord, width, height, settings::C_EnemtHealthRect, 1.5);
			}

			if (draw_tracer && enemyTeam != localPlayer.team) {
				drawLine(screenWidth / 2, screenHeight, screenHead.x, screenHead.y + height, settings::C_EnemyTracer, 1.5);
			}
			if (draw_tracer && enemyTeam == localPlayer.team && settings::teamTraser == 1) {
				drawLine(screenWidth / 2, screenHeight, screenHead.x, screenHead.y + height, settings::C_TeamTracer, 1.5);
			}

			if(settings::name == 1)
			{ 
			if (enemyTeam != localPlayer.team) {
				string name;
				for (int i = 0; i < 16; i++)
					name += driver::read_memory<char>(driv, currentController + CBasePlayerController::m_iszPlayerName + i);
				drawText(left_cord, top_cord - 21, name);
			}
			}

		}



	}







}

