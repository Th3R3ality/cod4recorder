#pragma once
#include "includes.h"
#include "global.h"

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	global::window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	global::window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return global::window;
}