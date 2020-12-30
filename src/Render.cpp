#include "Render.hpp"

LPDIRECT3DDEVICE9 Renderer::Init(HWND& window)
{
	// create our window class

	WNDCLASSEX WindowClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"lGuiMainWindow",
		NULL
	};

	Class = WindowClass;

	// initialize the window class

	RegisterClassEx(&WindowClass);

	// set refered window to newly created HWND.

	window = CreateWindow(
		WindowClass.lpszClassName,
		L"lGuiMainWindow",
		WS_POPUP,
		0,
		0,
		5,
		5,
		NULL,
		NULL,
		WindowClass.hInstance,
		NULL
	);

	// initialize D3D device for HWND.

	if (!CreateDeviceD3D(window)) {
		CleanupDeviceD3D();
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		return nullptr;
	}

	// before returning the device, we show the window etc.

	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	return this->g_pd3dDevice;
}

bool Renderer::CreateDeviceD3D(HWND hWnd)
{
	if ((this->g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		return false;
	}

	/* Set the D3DDevice up */

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));

	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; 

	auto device = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_d3dpp,
		&g_pd3dDevice
	);

	if (device < 0) {
		return false;
	}

	return true;
}

void Renderer::CleanupDeviceD3D()
{
	if (g_pd3dDevice) {
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}

	if (g_pD3D) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void Renderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT res = g_pd3dDevice->Reset(&g_d3dpp);

	if (res == D3DERR_INVALIDCALL) {
		IM_ASSERT(0);
	}

	ImGui_ImplDX9_CreateDeviceObjects();
}

void Renderer::Shutdown(HWND& window)
{
	this->CleanupDeviceD3D();
	DestroyWindow(window);
	UnregisterClass(Class.lpszClassName, Class.hInstance);
}
