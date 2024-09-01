#pragma once
#include <../external/ImGui/imgui.h>
#include <ImGui/imgui.h>
namespace settings
{

	int tab;

	//wh
	bool box;
	bool teamBox;
	bool name;
	bool fade1;
	bool fade2;
	bool fade3;
	bool fade4;

	bool health_bar;
	bool health_obvodka;

	bool skeleton;
	bool teamSkeleton;
	bool health;
	

	bool traser;
	bool teamTraser;

	//aim
	bool aimbot;
	bool visibleCheck;
	float Smooth = 0;
	int fov = 50;
	float fovSpedd = 1.5f;
	bool AimRCS;
	//bool aimAutoShoot = 1;


	bool trigger;
	int TriggerDelay = 90; // ms
	int ShotDuration = 200; // ms

	//misc
	bool bhop;

	//bomb
	bool bombTimer;

	//radar misc
	bool radar;
	pair <int, int> radar_center = { 150,150 };
	float radar_radius = 125;
	float radar_scale = 1;




	// colors

	int opasity = 50;

	bool RS_EnemyColorPicker = 0;
	ImColor C_EnemyBox = ImColor(255, 0, 0, 255);

	ImColor C_EnemySkelet = ImColor(255, 255, 255, 255);

	ImColor C_EnemyFade1 = ImColor(210, 154, 239, opasity);
	ImColor C_EnemyFade2 = ImColor(234, 141, 219, opasity);
	ImColor C_EnemyFade3 = ImColor(99, 222, 233, opasity);
	ImColor C_EnemyFade4 = ImColor(253, 0, 254, opasity);

	ImColor C_EnemyTracer = ImColor(0, 0, 0, 255);

	ImColor C_EnemyHealth = ImColor(50, 205, 50, 255);
	ImColor C_EnemtHealthRect = ImColor(0, 100, 0, 255);

	ImColor C_TeamBox = ImColor(17, 217, 200, 255);
	ImColor C_TeamSkelet = ImColor(255, 255, 255, 255);
	
	

	ImColor C_TeamTracer = ImColor(0, 0, 0, 255);
	ImColor C_TeamHealth = ImColor(50, 205, 50, 255);
	ImColor C_TeamHealthRect = ImColor(0, 100, 0, 255);
}

namespace bomb
{
	chrono::time_point<std::chrono::high_resolution_clock> timer;
	chrono::time_point<std::chrono::high_resolution_clock> delay_timer;
	bool timer_exists = 0;
}
