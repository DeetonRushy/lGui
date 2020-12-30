#pragma once

#include "Render.hpp"

enum GuiState {
	Running,
	Stopped
};

class Gui
{
public: // FUNCS

	static void OnStart();

	static void Init(HWND& window, Renderer rnd);
	static void SetSize(int _x, int _y);
	static void SetTitle(std::string new_title);
	static void SetState(GuiState state);

private: // VARS

	static bool enabled;
	static int x, y;
	static std::string title;
};

