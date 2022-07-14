
//#include "global.h"
#include "includes.h"
#include "globals.h"
#include "Overlay.h"
#include <lmcons.h>

ID3D11Device* ui::g_pd3dDevice = NULL;
ID3D11DeviceContext* ui::g_pd3dDeviceContext = NULL;
IDXGISwapChain* ui::g_pSwapChain = NULL;
ID3D11RenderTargetView* ui::g_mainRenderTargetView = NULL;



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
globals::aimbot aimbot;
globals::aimbotSettings aimbotSettings;
globals::visuals visuals;
globals::visualSettings visualSettings;

ImVec2 buttonSize = ImVec2(190, 20);
static int tabs = 3;

TCHAR name[UNLEN + 1];
DWORD size = UNLEN + 1;

/*
void importFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	ui::normalFont = io.Fonts->AddFontDefault();
	ui::customFont = io.Fonts->AddFontFromFileTTF("C:\\CodingConfig\\Hack-Regular.ttf", 16.0f);
}
*/

LPCWSTR windowtitle = L"OVERLAY";
LRESULT ui::wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wparam != SIZE_MINIMIZED) {
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lparam), (UINT)HIWORD(lparam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wparam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hwnd, message, wparam, lparam);
}
//#include "Utils/Utility.h"
HWND ui::create_window(HINSTANCE instance, const std::pair<int, int> size, const std::pair<int, int> pos /*= { 400, 400 }*/) {
	/*
	WNDCLASSEX wc;

	std::memset(&wc, 0, sizeof(wc));
	
	std::string r1 = "hello"; //Utility::Generator::randomString(16);
	std::string r2 = "this is test"; //Utility::Generator::randomString(16);

	std::wstring lClassName = std::wstring(r1.begin(), r1.end());
	std::wstring lWindowName = std::wstring(r2.begin(), r2.end());

	WNDCLASSEXW wClass;
	wClass.cbSize = sizeof(WNDCLASSEXW);
	wClass.style = 0;
	wClass.lpfnWndProc = wnd_proc;
	wClass.cbClsExtra = NULL;
	wClass.cbWndExtra = NULL;
	wClass.hInstance = nullptr;
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hbrBackground = nullptr;
	wClass.lpszMenuName = lWindowName.c_str();
	wClass.lpszClassName = lClassName.c_str();
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

	RegisterClassExW(&wClass);
	*/


	/*
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), ACS_TRANSPARENT, wnd_proc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("OVERLAYNAME"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, (L"OVERLAYNAME"), (L"OVERLAYNAME"), WS_POPUP, 0, 0, ScreenWidth, ScreenHeight, 0, 0, 0, 0);
	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);

	return CreateWindowExW(/*WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATENULL, (L"OVERLAYNAME"), (L"OVERLAYNAME"), WS_POPUP, 0, 0, ScreenWidth, ScreenHeight, 0, 0, 0, 0);
	*/
	
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), ACS_TRANSPARENT, wnd_proc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T("Overlay"), NULL };
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, windowtitle, windowtitle, WS_POPUP, 1, 1, ScreenWidth, ScreenHeight, 0, 0, 0, 0);;
	return hwnd;

	
	
	//return CreateWindowExW(/*WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED*/NULL, lClassName.c_str(), lWindowName.c_str(), WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, 0);

	//wc.cbSize = sizeof(WNDCLASSEX);
	//wc.style = CS_CLASSDC;
	//wc.lpfnWndProc = wnd_proc;
	//wc.hInstance = instance;
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = 0;
	//wc.lpszClassName = L"LoaderClass";

	//RegisterClassEx(&wc);

	//auto flag = WS_POPUP;
	///*flag &= ~WS_MAXIMIZEBOX;
	//flag &= ~WS_SIZEBOX;*/
	//return CreateWindowEx(NULL, wc.lpszClassName, L"client", flag, pos.first, pos.second, size.first, size.second, 0, 0, wc.hInstance, 0);

}
//#include "gui.h"

void ui::CreateRenderTarget() {
	ID3D11Texture2D* pBackBuffer;
	ui::g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	ui::g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &ui::g_mainRenderTargetView);
	pBackBuffer->Release();
}

void ui::CleanupRenderTarget() {
	if (ui::g_mainRenderTargetView) { ui::g_mainRenderTargetView->Release(); ui::g_mainRenderTargetView = NULL; }
}

bool ui::create_device(HWND hwnd) {

	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();

	return true;
}

void ui::cleanup_device() {
	//cleanupRender
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();

	//Cleanup Devices
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	
}

void dragSlider(std::string name, float* amount, float speed, float min, float max) {
	if (*amount < min)*amount = min;
	else if (*amount > max)*amount = max;
	//ImGui::DragFloat(name.c_str(), amount, min, max);
	ImGui::DragFloat(name.c_str(), amount, speed, min, max);

}

int aim_key;

static int keystatus = 0;
int keynames[] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10
};

void change_key(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				aim_key = i;
				keystatus = 0;
				return;
			}
		}
	}
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keynames))
		Items_ArrayGetter(keynames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = "Select Key";
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = "Press key to bind";
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}
void ui::aimbotTab()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Aimbot: \t\t\t  ");
	ImGui::SameLine();
	ImGui::Text("Settings:");
	//Code here for aimbot column
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1255f, 0.1255f, 0.1255f, 1.0f));
	ImGui::BeginChild("##aimbotPanel", ImVec2(153, 200), true);
	ImGui::Checkbox(" Aimbot", &aimbot.aimbotEnabled);
	ImGui::Checkbox(" Smooth", &aimbot.smoothEnabled);
	ImGui::Checkbox(" Accuracy", &aimbot.accuracyEnabled);
	ImGui::Checkbox(" Silent", &aimbot.silentAimEnabled);
	ImGui::Checkbox(" Rage Mode", &aimbot.rageModeEnabled);

	ImGui::PopStyleColor();
	//ImGui::PopFont();

	ImGui::EndChild();

	ImGui::SameLine();

	//Code here for settings column
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1255f, 0.1255f, 0.1255f, 1.0f));
	ImGui::BeginChild("##aimbotSettingsPanel", ImVec2(423, 200), true);

	ImGui::Text("FOV: \t\t");
	ImGui::SameLine();
	ImGui::SliderFloat("##fovSize", &aimbotSettings.fovSize, 0.0f, 1000.f, "FOV Size = %.2f");
	ImGui::Text("Smooth: \t ");
	ImGui::SameLine();
	ImGui::DragFloat("##smoothAmount", &aimbotSettings.smoothAmount, 1.0f, 0.00f, 100.00f);
	ImGui::Text("Aimkey: \t ");
	ImGui::SameLine();
	ImGui::Button("##aimkeyButton", ImVec2(274, 20));
	//HotkeyButton(aim_key, change_key, keystatus);
	ImGui::Text("Hitbox: \t ");
	ImGui::SameLine();
	ImGui::Combo("##hitbox", &aimbotSettings.selectedHitbox, aimbotSettings.hitbox, IM_ARRAYSIZE(aimbotSettings.hitbox));

	ImGui::PopStyleColor();
	ImGui::EndChild();

}
void ui::visualsTab()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Visuals: \t\t\t ");
	ImGui::SameLine();
	ImGui::Text("Settings:");
	//Code here for aimbot column
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1255f, 0.1255f, 0.1255f, 1.0f));
	ImGui::BeginChild("##visualsPanel", ImVec2(153, 200), true);
	ImGui::Checkbox(" Visuals", &visuals.visualsEnabled);
	ImGui::Checkbox(" Box", &visuals.boxEnabled);
	ImGui::Checkbox(" Corner", &visuals.cornerEnabled);
	ImGui::Checkbox(" Glow", &visuals.glowEnabled);
	ImGui::Checkbox(" Line", &visuals.lineEnabled);

	ImGui::PopStyleColor();
	//ImGui::PopFont();

	ImGui::EndChild();

	ImGui::SameLine();

	//Code here for settings column
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1255f, 0.1255f, 0.1255f, 1.0f));
	ImGui::BeginChild("##visualsSettingsPanel", ImVec2(423, 200), true);

	ImGui::Text("Distance: \t\t");
	ImGui::SameLine();
	ImGui::SliderFloat("##distanceSize", &visualSettings.distance, 0.0f, 1000.f, "Distance = %.2f");
	ImGui::Text("ESP Thickness:\t");
	ImGui::SameLine();
	ImGui::SliderFloat("##lineThickness", &visualSettings.lineThickness, 0.0f, 10.f, "Thickness = %.2f");

	ImGui::PopStyleColor();
	ImGui::EndChild();
}
void ui::miscTab()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Data Points: \t\t\t\t\t   ");
	ImGui::SameLine();
	ImGui::Text("Health Check:");

	ImGui::BeginChild("##dataPoints", ImVec2(250, 200), true);

	//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.84f, 1.0f), globals::COMPILED);
	ImGui::Spacing();
	ImGui::Text(globals::COMPILED);
	ImGui::Spacing();
	ImGui::Text("FPS: % .1f "/*1000.0f / ImGui::GetIO().Framerate*/, ImGui::GetIO().Framerate);
	ImGui::Spacing();
	ImGui::Text("TslEntity Count: ");
	ImGui::Spacing();
	ImGui::Text("posh#1729");
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("##healthCheck", ImVec2(326, 200), true);
	ImGui::Spacing();
	ImGui::Text("Base Address: ");
	ImGui::Spacing();
	ImGui::Text("GWorld: ");
	ImGui::Spacing();
	ImGui::Text("LocalPlayers: ");
	ImGui::Spacing();
	ImGui::Text("OwningGameInstance: ");
	ImGui::Spacing();
	ImGui::Text("Persistent Level: ");
	ImGui::Spacing();
	ImGui::Text("PlayerController: ");
	ImGui::Spacing();
	ImGui::Text("LocalPlayer: ");


	ImGui::EndChild();


}

void ui::defaultTab()
{
	GetUserNameA((TCHAR*)name, &size);
	ImGui::Spacing();
	ImGui::Text("Welcome");
	ImGui::SameLine();
	ImGui::Text(name);
	ImGui::Spacing();
	ImGui::Text("Made by posh#1729");
}

void ui::menu()
{
	ImGuiStyle& style = ImGui::GetStyle();
	//Window style
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
	//ImGui::PopStyleVar();
	style.WindowRounding = 10.f;
	style.FrameRounding = 2.f;
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0509f, 0.0509f, 0.0509f, 1.0f));
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0509f, 0.0509f, 0.0509f, 1.0f);
	//Button style
	/*
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1568f, 0.1568f, 0.1568f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1843f, 0.1843f, 0.1843f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1568f, 0.1568f, 0.1568f, 1.0f));
	*/
	

	style.Colors[ImGuiCol_Button] = ImVec4(0.1568f, 0.1568f, 0.1568f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1843f, 0.1843f, 0.1843f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1568f, 0.1568f, 0.1568f, 1.0f);

	//Combo list hitbox
	style.Colors[ImGuiCol_Header] = ImVec4(0.1765f, 0.1765f, 0.1765f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1765f, 0.1765f, 0.1765f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1765f, 0.1765f, 0.1765f, 1.0f);
	//style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2157f, 0.2157f, 0.2157f, 1.0f);


	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5882f, 0.0039f, 0.0000f, 1.00f);
	//style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0784f, 0.0784f, 0.0784f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1568f, 0.1568f, 0.1568f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1921f, 0.1921f, 0.1921f, 1.00f);
	//ImGui::SetCursorPosY(280.f);
	//ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.84f, 1.0f), COMPILED);

	ImGui::SetCursorPos(ImVec2(14, 20));
	if (ImGui::Button("! Aimbot", buttonSize))
	{
		tabs = 0;
	}
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(205, 20));
	if (ImGui::Button("$ Visuals", buttonSize))
	{
		tabs = 1;
		ImGui::SameLine();
	}
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(396, 20));

	if (ImGui::Button("% Misc", buttonSize))
	{
		tabs = 2;
	}

	switch (tabs)
	{
	case 0:
		ui::aimbotTab();
		break;
	case 1:
		ui::visualsTab();
		break;
	case 2:
		ui::miscTab();
		break;
	default:
		ui::defaultTab();
		break;

	}
}