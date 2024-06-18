#pragma once

namespace userinterface
{
	inline int selectedRecordingIndex = -1;
	inline int replayCountDown = 0;
	inline int recordCountDown = 0;
	inline bool showMenu = true;
	void Draw();
	void ToggleMenu(int override = 0);
};