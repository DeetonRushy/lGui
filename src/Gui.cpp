#include "Gui.hpp"

void Gui::Init(HWND& window, Renderer rnd)
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr; // we don't want that shit ini file popping up
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark(); // nice look :)

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(rnd.g_pd3dDevice);

	DWORD wFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

	RECT rScreen;
	GetWindowRect(GetDesktopWindow(), &rScreen);
	auto x = float(rScreen.right  - this->x) / 2.f;
	auto y = float(rScreen.bottom - this->y) / 2.f;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0u, 0u, PM_REMOVE)) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{ // FRAME_START

			ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(600, 400));
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::Begin(title.c_str(), &enabled, wFlags);
			{ // DRAW_START

				if (ImGui::Button("End", ImVec2(50, 50))) {
					enabled = false;
				}


			} // DRAW_END

			ImGui::End();

		} // FRAME_END

		ImGui::EndFrame();

		rnd.g_pd3dDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0,
			1.0f,
			0
		);

		if (rnd.g_pd3dDevice->BeginScene() >= 0) {

			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			rnd.g_pd3dDevice->EndScene();
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		HRESULT hRes = rnd.g_pd3dDevice->Present(0, 0, 0, 0);

		if (hRes == D3DERR_DEVICELOST && rnd.g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			rnd.ResetDevice();
		}

		if (!enabled) {
			msg.message = WM_QUIT;
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	rnd.Shutdown(window);
}

void Gui::OnStart()
{
	x = 600;
	y = 400;
	title = "lGui";
	SetState(Running);
}

void Gui::SetSize(int _x, int _y)
{
	if ((x != _x && y != _y) && !(_x < 0) && !(_y < 0)) {
		y = _y;
		x = _x;

		size_set = true;
	}
}

void Gui::SetTitle(std::string new_title)
{
	if (!(new_title.compare(title.c_str()))) {

		title = new_title;

		title_set = true;
	}
}

void Gui::SetState(GuiState state)
{
	switch (state) {
	case Running:
		enabled = true;
	case Stopped:
		enabled = false;
	default:
		return;
	}
}
