#pragma once
#include "../include.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <ImGui/imgui_internal.h>
#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")





extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
		return 0L;
	}

	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0L;
	}


	return DefWindowProc(window, message, w_param, l_param);
}

struct Animator
{

	float alpha;
	bool tick;
	float speed;

};

inline void AnimateContent(Animator& animator)
{
	const int limit = 255;
	if (animator.tick || animator.alpha == limit) {
		animator.tick == true;
		if (!(animator.alpha <= 0))
			animator.alpha -= animator.speed;
		else if (animator.alpha <= 0)
			animator.tick ^= 1;
	}

	else if (!animator.tick || animator.alpha != limit)
	{
		animator.tick = false;
		if (!(animator.alpha >= limit))
			animator.alpha += animator.speed;
		else if (animator.alpha >= limit)
			animator.tick ^= 1;

	}

}

inline void CenterButtons(vector<string> names, vector<int> indexs, int& selected_index) {

	vector<ImVec2> sizes = {};
	float total_area = 0.0f;

	const auto& style = ImGui::GetStyle();

	for (string& name : names) {
		const ImVec2 label_size = ImGui::CalcTextSize(name.c_str(), 0, true);
		ImVec2 size = ImGui::CalcItemSize(ImVec2(), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		size.x += 45.5f;
		size.y += 15.f;

		sizes.push_back(size);
		total_area += size.x;
	}

	ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (total_area / 2));
	for (uint32_t i = 0; i < names.size(); i++) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

		if (selected_index == indexs[i]) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImColor(3, 179, 179, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(3, 179, 179, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(3, 179, 179, 255).Value);

			if (ImGui::Button(names[i].c_str(), sizes[i]))
				selected_index = indexs[i];
			ImGui::PopStyleColor(3);
		}
		else {
			if (ImGui::Button(names[i].c_str(), sizes[i]))
				selected_index = indexs[i];
		}

		ImGui::PopStyleVar();
		if (i != (names.size() -1))
			ImGui::SameLine();

	}

}

inline void CenterText(const char* format, const float y_padding = 0.0f, ImColor color = ImColor(255, 255, 255)) {
	const ImVec2 text_size = ImGui::CalcTextSize(format);

	ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (text_size.x / 2));
	if (y_padding > 0.0f)
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y_padding);
	ImGui::TextColored(color, format);
}

inline void	CustomCheckBox(const char* format, bool* value) {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.5f, 1.5f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(47, 77, 83).Value);

	ImGui::Checkbox(format, value);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::Dummy(ImVec2());
}

inline ID3D11ShaderResourceView* ImageResource = nullptr;

inline bool LoadImageByMemmory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result)
{
	D3DX11_IMAGE_LOAD_INFO information;
	ID3DX11ThreadPump* thread = nullptr;

	auto hres = D3DX11CreateShaderResourceViewFromMemory(device, image, image_size, &information, thread, result, 0);
	return (hres == S_OK);
}