#include "includes.h"
#include "global.h"
#include "userinterface.h"
#include "timing.h"
#include "window.h"
#include "input.h"
#include "cod4/include.h"
#include "savefile.h"

#include "imgui/include.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
WNDPROC orig_WndProc;

DWORD WINAPI MainThread(HMODULE hModule)
{
	global::baseAddress = (DWORD)GetModuleHandleA(NULL);
	global::window = GetProcessWindow();

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	printf("hello\n");
	printf("BaseAddress : %x\n", global::baseAddress);

	savefile::Init();
	timing::Init();
	hooks::Init();

	orig_WndProc = (WNDPROC)SetWindowLongPtr(global::window, GWL_WNDPROC, (LONG_PTR)WndProc);

	while (!global::wantsQuit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		if (global::wantsRecord && !recorder::isRecording && !replayer::isReplaying)
		{
			recorder::NewRecording();
		}
		if (global::wantsReplay && !replayer::isReplaying && !recorder::isRecording)
		{
			if (!replayer::autoReplay)
				replayer::Play(userinterface::selectedRecordingIndex);
			else
			{
				timing::Wait(SECOND * 0.5f);
				PostMessageA(global::window, WM_KEYDOWN, 'E', MapVirtualKey('E', MAPVK_VK_TO_VSC));
				PostMessageA(global::window, WM_KEYUP, 'E', MapVirtualKey('E', MAPVK_VK_TO_VSC));
				timing::Wait(SECOND * 0.25f);

				replayer::Play(userinterface::selectedRecordingIndex, false, true);
			}
		}
		global::wantsRecord = false;
		global::wantsReplay = false;

		if (replayer::autoReplay)
			global::wantsReplay = true;
	}

	hooks::Detach();

	HWND consoleHwnd = GetConsoleWindow();

	FreeConsole();
	PostMessageA(consoleHwnd, WM_CLOSE, 0, 0);

	SetWindowLongPtr(global::window, GWL_WNDPROC, (LONG_PTR)orig_WndProc);
	FreeLibraryAndExitThread(hModule, 0);

	return TRUE;
}
BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		if (auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hMod, 0, nullptr))
			CloseHandle(handle);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	input::WndProc(hWnd, uMsg, wParam, lParam);

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(orig_WndProc, hWnd, uMsg, wParam, lParam);
}
