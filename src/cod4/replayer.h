#pragma once

namespace replayer
{
	inline int selectedRecordingIndex = -1;
	inline size_t replayIndex = 0;
	inline bool isReplaying = false;
	inline bool autoReplay = false;

	void Play(int recordingIndex, bool autoHideMenu = true, bool skipDelay = false);
}