#include "replayer.h"

#include "../global.h"
#include "../userinterface.h"
#include "../timing.h"
#include "recorder.h"

namespace replayer
{
	void Play(int recordingIndex, bool autoHideMenu, bool skipDelay)
	{
		global::wantsReplay = false;

		if (!skipDelay)
		{
			printf("Waiting 3 Seconds before playing...\n");

			for (int i = 3; i > 0; i--)
			{
				userinterface::replayCountDown = i;
				printf("%i\n", i);
				timing::Wait(SECOND);
			}
			userinterface::replayCountDown = 0;
			printf("0\n");
		}
		printf("replaying\n");

		if (autoHideMenu)
			userinterface::showMenu = false;

		isReplaying = true;
	}
	void Stop()
	{
		if (!isReplaying) return;
		replayIndex = 0;
		isReplaying = false;
	}
}
