#include "input.h"
#include "userinterface.h"
#include "global.h"
#include "cod4/recorder.h"
#include "cod4/replayer.h"

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

			if (recorder::isRecording || replayer::isReplaying)
			{
				if (wParam == VK_F4)
				{
					recorder::StopRecordingSegment();
					replayer::autoReplay = false;
					replayer::Stop();
				}
			}
			else
			{
				if (recorder::currentSegment.size() == 0)
				{
					if (wParam == VK_F3)
					{
						if (recorder::currentRecording == nullptr)
							recorder::NewRecording();
						else
							recorder::WantNewSegment();
					}
					if (wParam == VK_F4)
						recorder::currentRecording = nullptr;
				}
				else
				{
					if (wParam == VK_F3)
						recorder::SaveSegmentToCurrentRecording();
					if (wParam == VK_F4)
						recorder::DiscardSegment();
				}
			}
			break;
		}
		case WM_KEYUP:
		{
			break;
		}
		}
	}
}