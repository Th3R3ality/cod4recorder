#pragma once
#include <Windows.h>
#include "cod4/types/include.h"

namespace global
{
	inline unsigned int createcmdcount = 0;
	inline usercmd_t cmd = {};
	inline bool viewcmd = false;

	inline DWORD baseAddress = 0x0;
	inline HWND window = NULL;
	inline bool wantsQuit = false;
	inline bool wantsReplay = false;
	inline bool wantsRecord = false;
	inline bool debugPrints = false;
}
