
#include "includes.h"
#include "Overlay.h"
#include "globals.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


void hideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void showConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool isOpen = false;
void ChangeClickability(bool canclick, HWND ownd)
{
    long style = GetWindowLong(ownd, GWL_EXSTYLE);
    if (canclick) {
        style &= ~WS_EX_LAYERED;
        //SetWindowLong(ownd, GWL_EXSTYLE, style);
        SetForegroundWindow(ownd);  
        SetWindowLongA(ownd, GWL_EXSTYLE, (WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));

        //windowstate = 1;
    }
    else {
        style |= WS_EX_LAYERED;
        SetWindowLong(ownd, GWL_EXSTYLE, style);
        //SetWindowLongA(ownd, GWL_EXSTYLE, GetWindowLong(ownd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetWindowLongA(ownd, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));

        //windowstate = 0;
    }
}



int main()
{
    hideConsole();
    //ui::importFonts();
    auto hwnd = ui::create_window(GetModuleHandle(NULL), { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });

    if (!ui::create_device(hwnd)) {
        MessageBoxA(0, "internal graphics error, please check your video drivers.", "Failed", MB_OK);
        return 0;
    }
    MARGINS margin = { -1 };

    SetWindowLongA(hwnd, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
    DwmExtendFrameIntoClientArea(hwnd, &margin);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetStyle().WindowRounding = 0.f;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(ui::g_pd3dDevice, ui::g_pd3dDeviceContext);

    MSG msg;
    std::memset(&msg, 0, sizeof(msg));

    while (msg.message != WM_QUIT) {
        if (GetAsyncKeyState(VK_TAB) & 1) {
            isOpen = !isOpen;
        }

        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        /*
        if (ChangeClickability)
            SetWindowLongA(hwnd, GWL_EXSTYLE, (WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
        else
            SetWindowLongA(hwnd, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
            */
                
        /*
        ImGui::GetIO().Fonts->Clear();
        ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\CodingConfig\\Hack-Regular.ttf", 16.0f);
        ImGuiIO& io = ImGui::GetIO();

        */
        /*
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\fonts\\Clear.ttf", 16.0f); 
        io.Fonts->Build();      
        */
        /*
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        */

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //ImGui Menu Here

        if (isOpen) {
            ChangeClickability(true, hwnd);
            ImGui::SetNextWindowSize(ImVec2(600, 300));

            ImGui::Begin(
                "Menu",
                &isOpen,
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoScrollbar
            );
            //Application average % .3f ms / frame

            ui::menu();
            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


            ImGui::End();
        }
        else {
            ChangeClickability(false, hwnd);
        }



        ImGui::Render();
        ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        ui::g_pd3dDeviceContext->OMSetRenderTargets(1, &ui::g_mainRenderTargetView, NULL);
        ui::g_pd3dDeviceContext->ClearRenderTargetView(ui::g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ui::g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    ui::cleanup_device();
    DestroyWindow(hwnd);
}