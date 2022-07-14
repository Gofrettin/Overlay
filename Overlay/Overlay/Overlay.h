#pragma once

#include "imgui.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <d3d11.h>

namespace ui {


	extern ID3D11Device* g_pd3dDevice;
	extern ID3D11DeviceContext* g_pd3dDeviceContext;
	extern IDXGISwapChain* g_pSwapChain;
	extern ID3D11RenderTargetView* g_mainRenderTargetView;

	LRESULT wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	HWND create_window(HINSTANCE instance, const std::pair<int, int> size, const std::pair<int, int> pos = { 0, 0 });

	void CreateRenderTarget();
	void CleanupRenderTarget();

	bool create_device(HWND hwnd);
	void cleanup_device();

	//

	//void importFonts();
	//tabs
	void aimbotTab();
	void visualsTab();
	void miscTab();
	void defaultTab();
	//main menu
	void menu();
	





};