#pragma once

#include "Render.hpp"

class Gui
{
public:

	static void Init(HWND& window, Renderer rnd);

private:

	static bool enabled;
};

