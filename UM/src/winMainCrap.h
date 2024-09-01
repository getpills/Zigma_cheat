#pragma once

#include "include.h"


#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include "ImGui/imgui_internal.h"
#include <dwmapi.h>
#include <d3d11.h>
#include <windowsx.h>

#include "../external/ImGuiForCheat/style.h"
#include "../external/ImGuiForCheat/font.h"
#include "../external/ImGuiForCheat/font_awesome.h"
#include "../external/ImGuiForCheat/photos.h"
#include <ImGuiForCheat/font_awesome.cpp>

#include "../src/settings/settings.h"

#include "../src/features/wh/renderer.h"
#include "../src/features/triggerbot/triggerbot.h"
#include "graphics/ImGuiWindowCrap.h"
#include "settings/settings.h"
#include "../src/features/aimbot/aimbot.h"
#include "features/bhop/bhop.h"


#include "features/bomb/bombTimer.h"



extern int screenWidth1 = GetSystemMetrics(SM_CXSCREEN);
extern int screenHeight1 = GetSystemMetrics(SM_CYSCREEN);




INT APIENTRY WinCrap(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{
	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = instance;
	wc.lpszClassName = L"External Overlay Class";

	RegisterClassExW(&wc);

	const HWND window = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		wc.lpszClassName,
		L"ZIGma Cheats | FeluxaTopG",
		WS_POPUP,
		0,
		0,
		screenWidth1,
		screenHeight1,
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);
	/*const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"ZIGma Cheats | FeluxaTopG",
		WS_POPUP,
		0,
		0,
		screenWidth1,
		screenHeight1,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
	*/
	

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT client_area{};
		GetClientRect(window, &client_area);

		RECT window_area{};
		GetWindowRect(window, &window_area);

		POINT diff{};
		ClientToScreen(window, &diff);

		const MARGINS margins{
			window_area.left + (diff.x - window_area.left),
			window_area.top + (diff.y - window_area.top),
			client_area.right,
			client_area.bottom
		};

		DwmExtendFrameIntoClientArea(window, &margins);

	}

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	D3D_FEATURE_LEVEL level{};


	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	ID3D11Texture2D* back_buffer{ nullptr };
	swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

	if (back_buffer)
	{
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else {
		return 1;
	}

	ShowWindow(window, cmd_show);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGuiStyle& style = ImGui::GetStyle();
	SetupImGuiStyle();
	
	
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig font;
	font.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF((void*)rawData, sizeof(rawData), 18.5f, &font);
	
	
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;
	
	ImFont* icons_font = nullptr;
	
	
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 18.5f, &icons_config, icon_ranges);


	bool result = LoadImageByMemmory(device, GH_Logo, sizeof(GH_Logo), &ImageResource);
	if (!result)
		return false;



	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, device_context);


	bool RenderMenu = false;
	bool show = false;
	bool running = true;
	
	
	

	while (running)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				running = false;
			}

		}

		if (!running) {
			break;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		int mode = 0;
		int index = 0;
		
		

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			RenderMenu = !RenderMenu;
			show = !show;

			if (RenderMenu) {
				SetWindowLong(window, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_TOPMOST);
			}
			else {
				SetWindowLong(window, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED);
			}

			
		}

		// code here.
		//-------------------------------------------------------------------
		
		if (show)
		{
			ImGui::Begin("Porno | ZIGma Cheats", &RenderMenu, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			static Animator animator{ 0, false, 1.21f };
			
			AnimateContent(animator);
			ImGui::SetNextWindowSize(ImVec2(900, 450));
			
			
			ImVec2 window_pos{ ImGui::GetWindowPos() };
			ImVec2 window_size{ ImGui::GetWindowSize() };
			ImVec2 cursor_pos{ ImGui::GetCursorPos() };
			ImVec2 image_size{ 1020 / 2, 1044 / 2.7  };
			
			
			
				ImGui::SameLine();
				
				CenterButtons({ ICON_FA_CROSSHAIRS" Aimbot", ICON_FA_EYE" Visuals", ICON_FA_MINUS_CIRCLE" MISC", ICON_FA_ADDRESS_CARD" About"}, {0, 1, 2, 3}, settings::tab);
				
				ImGui::Separator();
				
				

				ImGui::BeginChild("##left_side", ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, ImGui::GetContentRegionAvail().y));
				
				if (settings::tab == 0)
				{
					
					CustomCheckBox("Trigger", &settings::trigger);
					
					CustomCheckBox("Aimbot", &settings::aimbot);
					
					if (settings::aimbot == 1)
					{
						CustomCheckBox("Recoil Control System", &settings::AimRCS);
						CustomCheckBox("Visible Check", &settings::visibleCheck);
						
						
						ImGui::SliderFloat("Aim Speed", &settings::fovSpedd, 0.5f, 1.5f);
						

						ImGui::SliderFloat("Aim Smooth",  &settings::Smooth, 0.f, 4.f);
						if (settings::Smooth > 0 && settings::Smooth < 0.1)
							settings::Smooth = 0;
						if (settings::Smooth > 0.1 && settings::Smooth < 0.4)
							settings::Smooth = 0.4;
						if (settings::Smooth > 0.4 && settings::Smooth < 0.6)
							settings::Smooth = 0.5;

						ImGui::SliderInt("Fov", &settings::fov, 0, 500);
						
					}
				}
				if (settings::tab == 1)
				{

					CustomCheckBox("2D box", &settings::box);
					if (settings::box == 1)
					{
						
						ImGui::SameLine();
						CustomCheckBox("Team Box", &settings::teamBox);
						ImGui::SameLine();
						CustomCheckBox("Fade 1", &settings::fade1);
						
					}
					else {
						settings::fade1 = 0;
						
					}
					
					CustomCheckBox("Name", &settings::name);

					CustomCheckBox("skeleton", &settings::skeleton);
					if (settings::skeleton == 1)
					{
						
						ImGui::SameLine();
						CustomCheckBox("Team skeleton", &settings::teamSkeleton);
					}
					else {
						settings::teamSkeleton = 0;
						
					}
					CustomCheckBox("Health bar", &settings::health);
					if (settings::health == 1)
					{
						

					}
					else {
						settings::health_bar = 0;
						settings::health_obvodka = 0;
					}
					

					CustomCheckBox("Tracer", &settings::traser);
					if (settings::traser == 1)
					{
						ImGui::SameLine();
						CustomCheckBox("Team Tracer", &settings::teamTraser);

					}
					else {
						settings::teamTraser = 0;
					}
					
				}
				if (settings::tab == 2)
				{
					CustomCheckBox("Bunny Hop", &settings::bhop);
					CustomCheckBox("Radar Hack", &settings::radar);
					if (settings::radar == 1)
					{
						ImGui::SliderFloat("Radar Scale", &settings::radar_scale, 0.5f, 1.5f);
						ImGui::SliderFloat("Radar Radius", &settings::radar_radius, 100, 200);
						ImGui::SliderInt("Radar Center X", &settings::radar_center.first, 100, 300);
						ImGui::SliderInt("Radar Center Y", &settings::radar_center.second, 100, 300);
					}
					CustomCheckBox("Bomb Timer", &settings::bombTimer);
				}
				if (settings::tab == 3)
				{
					ImGui::Image((void*)ImageResource, image_size);
				}
		
				ImGui::EndChild();


				ImGui::SameLine();
				

				ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

				
				ImGui::SameLine();

				ImGui::BeginChild("##right_side", ImVec2(ImGui::GetContentRegionAvail().x / 1.0f, ImGui::GetContentRegionAvail().y));
				if (settings::tab == 1){
					if (settings::box == 1) {

						ImGui::Text("Enemy Box");
						ImGui::SameLine();

						if (ImGui::ColorButton("Enemy Color", settings::C_EnemyBox))
						{
							ImGui::OpenPopup("ColorPickerPopup");

						}

						if (ImGui::BeginPopup("ColorPickerPopup")) {
							ImGui::ColorPicker4("", (float*)&settings::C_EnemyBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}
						
						if (settings::teamBox == 1) {
							ImGui::Text("Team Box");
							ImGui::SameLine();
							

							if (ImGui::ColorButton("Team Box Color", settings::C_TeamBox))
							{
								ImGui::OpenPopup("ColorPickerPopupTeamBox");

							}

							if (ImGui::BeginPopup("ColorPickerPopupTeamBox")) {
								ImGui::ColorPicker4("", (float*)&settings::C_TeamBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
								ImGui::EndPopup();
							}
						}

						if (settings::fade1 == 1)
						{
							


							ImGui::Text("Fade 1");
							ImGui::SameLine();
							if (ImGui::ColorButton("Enemy ColorFade1", settings::C_EnemyFade1))
							{
								ImGui::OpenPopup("ColorPickerPopupFade1");

							}

							if (ImGui::BeginPopup("ColorPickerPopupFade1")) {
								ImGui::ColorPicker4("", (float*)&settings::C_EnemyFade1, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
								ImGui::EndPopup();
							}
							ImGui::SameLine();
							ImGui::Text("Fade 2");
							ImGui::SameLine();
							
							if (ImGui::ColorButton("Enemy ColorFade2", settings::C_EnemyFade2))
							{
								ImGui::OpenPopup("ColorPickerPopupFade2");

							}

							if (ImGui::BeginPopup("ColorPickerPopupFade2")) {
								ImGui::ColorPicker4("", (float*)&settings::C_EnemyFade2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
								ImGui::EndPopup();
							}
							ImGui::SameLine();
							ImGui::Text("Fade 3");
							ImGui::SameLine();
							if (ImGui::ColorButton("Enemy ColorFade3", settings::C_EnemyFade3))
							{
								ImGui::OpenPopup("ColorPickerPopupFade3");

							}

							if (ImGui::BeginPopup("ColorPickerPopupFade3")) {
								ImGui::ColorPicker4("", (float*)&settings::C_EnemyFade3, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
								ImGui::EndPopup();
							}
							ImGui::SameLine();
							ImGui::Text("Fade 4");
							ImGui::SameLine();
							if (ImGui::ColorButton("Enemy ColorFade4", settings::C_EnemyFade4))
							{
								ImGui::OpenPopup("ColorPickerPopupFade4");

							}

							if (ImGui::BeginPopup("ColorPickerPopupFade4")) {
								ImGui::ColorPicker4("", (float*)&settings::C_EnemyFade4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
								ImGui::EndPopup();
							}

							ImGui::SliderInt("Opasity", &settings::opasity, 1, 255);

							settings::C_EnemyFade1.Value.w = settings::opasity / 255.f;
							settings::C_EnemyFade2.Value.w = settings::opasity / 255.f;
							settings::C_EnemyFade3.Value.w = settings::opasity / 255.f;
							settings::C_EnemyFade4.Value.w = settings::opasity / 255.f;
							
						}
						
							
						

					} // box

					if (settings::health == 1) {
						
						ImGui::Text("Box Color");
						ImGui::SameLine();
						if (ImGui::ColorButton("Box Color", settings::C_EnemyHealth))
						{
							ImGui::OpenPopup("ColorPickerPopupHealthBar");

						}

						if (ImGui::BeginPopup("ColorPickerPopupHealthBar")) {
							ImGui::ColorPicker4("", (float*)&settings::C_EnemyHealth, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text("Box Outline");
						ImGui::SameLine();
						if (ImGui::ColorButton("Box Obvodka", settings::C_EnemtHealthRect))
						{
							ImGui::OpenPopup("ColorPickerPopupHealthObvodka");

						}

						if (ImGui::BeginPopup("ColorPickerPopupHealthObvodka")) {
							ImGui::ColorPicker4("", (float*)&settings::C_EnemtHealthRect, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}

					}//health

					if (settings::skeleton == 1) {
						ImGui::Text("Enemy Skeleton");
						ImGui::SameLine();

						if (ImGui::ColorButton("Enemy Skelet", settings::C_EnemySkelet))
						{
							ImGui::OpenPopup("ColorPickerPopupEnemySkelet");

						}

						if (ImGui::BeginPopup("ColorPickerPopupEnemySkelet")) {
							ImGui::ColorPicker4("", (float*)&settings::C_EnemySkelet, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text("Team Skeleton");
						ImGui::SameLine();
						if (ImGui::ColorButton("Team Skelet", settings::C_TeamSkelet))
						{
							ImGui::OpenPopup("ColorPickerPopupTeamSkelet");

						}

						if (ImGui::BeginPopup("ColorPickerPopupTeamSkelet")) {
							ImGui::ColorPicker4("", (float*)&settings::C_TeamSkelet, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}

					}// skeleton

					if (settings::traser == 1) {

						ImGui::Text("Enemy Traser");
						ImGui::SameLine();

						if (ImGui::ColorButton("Enemy Traser1", settings::C_EnemyTracer))
						{
							ImGui::OpenPopup("ColorPickerPopupEnemyTracer");

						}

						if (ImGui::BeginPopup("ColorPickerPopupEnemyTracer")) {
							ImGui::ColorPicker4("", (float*)&settings::C_EnemyTracer, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text("Team Tracer");
						ImGui::SameLine();
						if (ImGui::ColorButton("Team Tracer1", settings::C_TeamTracer))
						{
							ImGui::OpenPopup("ColorPickerPopupTeamTracer");

						}

						if (ImGui::BeginPopup("ColorPickerPopupTeamTracer")) {
							ImGui::ColorPicker4("", (float*)&settings::C_TeamTracer, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
							ImGui::EndPopup();
						}

					}//traser


				} // tab

				if (settings::tab == 3)
				{


				
					CenterText( ICON_FA_ADDRESS_CARD " Dev: Getpills | FeluxaTopG", 95.0f, ImColor(255, 255, 255, (int)animator.alpha));
				
				CenterText(" \nGratitude: ask17801", 95.0f, ImColor(255, 255, 255, (int)animator.alpha));

				}
				ImGui::EndChild();
				
				
				

				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				draw_list->AddRect({ window_pos.x - 1, window_pos.y - 1 }, { window_pos.x + window_size.x + 1, window_pos.y + window_size.y + 1 },
					ImColor(47, 77, 83, (int)animator.alpha), 4.0f, 0, 5.0f);
			

				ImGui::End();
		}
		
		if (settings::box == 1) {
			mode += 2;
		}

		if (settings::skeleton == 1) {
			mode += 1;
		}
		if (settings::health == 1) {
			mode += 4;
		}
		if (settings::traser == 1) {
			mode  += 8;
		}
		if (settings::name == 1) {
			mode += 16;
		}
		
		


		Wall_Hack(mode);

		if (GetAsyncKeyState(VK_XBUTTON1) && settings::trigger == 1) {
			triggerbot();
		}
		
		if (settings::aimbot == 1) {

			aimbot();
		}
		//RecoilControl();

		//misc
		if(settings::bombTimer == 1)
		{ 
		bomb_timer2();
		}
		// radar v draw.h
		//FastStop();

		
		
		if (GetAsyncKeyState(VK_XBUTTON2) && settings::bhop == 1) {
			bhop();
		}
		
		
		
		
		

		Sleep(5);
		
		//-------------------------------------------------------------------
		ImGui::Render();

		constexpr float color[4]{ 0, 0, 0, 0 };
		device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
		device_context->ClearRenderTargetView(render_target_view, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(0U, 0U);








	} //while running

	

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();


	if (swap_chain)
	{
		swap_chain->Release();
	}

	if (device_context)
	{
		device_context->Release();
	}

	if (device) {
		device->Release();
	}

	if (render_target_view) {
		render_target_view->Release();
	}

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);





	return 0;

}