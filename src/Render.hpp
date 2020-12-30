#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// This object handles all DirectX devices & window related operations.

class Renderer
{
public: // FUNCS

	LPDIRECT3DDEVICE9 Init(HWND& window, const wchar_t* class_name);

    LPDIRECT3DDEVICE9      g_pd3dDevice;
    D3DPRESENT_PARAMETERS  g_d3dpp;
    LPDIRECT3D9            g_pD3D;

    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void ResetDevice();

    void Shutdown(HWND& window);

private:

    WNDCLASSEX Class;
};

// WndProc

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    Renderer worker{};

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    auto device = worker.g_pd3dDevice;
    auto params = worker.g_d3dpp;

    switch (msg)
    {
    case WM_SIZE:
        if (device != NULL && wParam != SIZE_MINIMIZED)
        {
            params.BackBufferWidth = LOWORD(lParam);
            params.BackBufferHeight = HIWORD(lParam);
            worker.ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}