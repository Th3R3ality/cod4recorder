#pragma once
#include "recorder.h"

namespace replayer
{
	inline int selectedRecordingIndex = -1;
	inline size_t recordingIndex = 0;
	inline bool isReplaying = false;
	inline bool autoReplay = false;

	void PlaySelectedRecording(bool autoHideMenu = true, bool skipDelay = false);
	void SelectRecordingByIndex(int index);
	void Stop();
	void ModifyNextCommand(usercmd_t* const cmd);
}