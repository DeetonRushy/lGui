#include "Render.hpp"
#include "Gui.hpp"

HWND g_Window;

int main()
{
	// OnStart is important, it initializes the static members of the Gui class.
	// This function allows you to optionally set the size, title & state.

	Gui::OnStart();

	// Creating an object of Renderer & calling Init to initialize all the needed D3D9 devices.
	// This also creates our window, window classes & DirectX devices.

	Renderer worker;
	worker.Init(g_Window, L"lGuiMainWindowClass"); // change the window class name if you really want to.

	// Optional functions if OnStart was called.
	// If OnStart isn't called, they're required.
	// They're very self explanitory.

	Gui::SetSize(800, 600);
	Gui::SetTitle("lGui");
	Gui::SetState(Running);

	// This initializes the GUI & starts drawing the GUI.
	// The reference to our initialized Renderer is passed along with the global
	// variable 'g_Window' is passed which our renderer created.

	Gui::Init(g_Window, worker);
}

