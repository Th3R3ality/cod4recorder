#include "includes.h"
#include "global.h"
#include "userinterface.h"
#include "timing.h"
#include "window.h"
#include "input.h"
#include "cod4/include.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitImGui(LPDIRECT3DDEVICE9 pDevice);
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);

EndScene oEndScene = NULL;
WNDPROC oWndProc;


DWORD WINAPI MainThread(HMODULE hModule)
{
	global::baseAddress = (DWORD)GetModuleHandleA(NULL);
	timing::Init();

	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				global::window = GetProcessWindow();
			while (global::window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(global::window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	printf("hello\n");
	printf("%x", global::baseAddress);

	hooks::Init();

	while (!global::wantsQuit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		if (global::wantsRecord && !recorder::isRecording && !replayer::isReplaying)
		{
			recorder::BeginRecording();
		}
		if (global::wantsReplay && !replayer::isReplaying && !recorder::isRecording)
		{
			if (!replayer::autoReplay)
				replayer::Play(userinterface::selectedRecordingIndex);
			else
			{
				userinterface::ToggleMenu(-1);
				timing::Wait(SECOND * 0.25f);
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

	kiero::shutdown();

	HWND consoleHwnd = GetConsoleWindow();

	FreeConsole();
	PostMessageA(consoleHwnd, WM_CLOSE, 0, 0);

	SetWindowLongPtr(global::window, GWL_WNDPROC, (LONG_PTR)oWndProc);
	FreeLibraryAndExitThread(hModule, 0);

	return TRUE;
}

bool init = false;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	userinterface::Draw();
	ImGui::GetIO().MouseDrawCursor = false;

	return oEndScene(pDevice);
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

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(global::window);
	ImGui_ImplDX9_Init(pDevice);
}