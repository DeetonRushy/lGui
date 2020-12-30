#include "Render.hpp"
#include "Gui.hpp"

HWND g_Window;

int main()
{
	Renderer worker;
	worker.Init(g_Window);
	Gui::Init(g_Window, worker);
}

