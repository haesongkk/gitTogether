#include "framework.h"
#include "hsImGui.h"

hsImGui::hsImGui(HWND _hWnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDC)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX11_Init(_pDevice, _pDC);
}

void hsImGui::Init()
{
    
}

void hsImGui::Update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(200, 200));

    ImGui::Begin("settings");

    ImGui::Text("color");
    //ImGui::ColorEdit3("##color", (float*)&(m_dirLight.color));

    ImGui::Text("direction");
    //ImGui::DragFloat3("##dir", (float*)&(m_dirLight.dir), 0.1f, -1.f, 1.f);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void hsImGui::Finalize()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
