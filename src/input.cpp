#include "input.h"
#include "userinterface.h"
#include "global.h"

namespace input
{
	void WndProc(const HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
		{
			if (wParam == VK_INSERT)
				userinterface::ToggleMenu();
			if (wParam == VK_DELETE)
				global::wantsQuit = true;

			break;
		}
		case WM_KEYUP:
		{
			break;
		}
		}
	}
}