#pragma once
#include "recorder.h"

namespace replayer
{
	inline size_t recordingIndex = 0;
	inline bool isReplaying = false;
	inline bool autoReplay = false;
	inline int startServertime = 0;

	void PlaySelectedRecording(bool autoHideMenu = true, bool skipDelay = false);
	void Stop(usercmd_t* const cmd = nullptr);

	void WantReplay(bool repeating = false);
	bool WantsToReplay();
	inline bool CanStartReplaying()
	{
		return !replayer::isReplaying && !recorder::isRecording;
	}
	void ModifyNextCommand(usercmd_t* const cmd);
}