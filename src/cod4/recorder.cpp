#include "recorder.h"

#include <iostream>
#include <format>

#include "../timing.h"
#include "../userinterface.h"

namespace recorder
{
	void BeginRecording()
	{
		printf("Waiting 3 Seconds before recording...\n");
		for (int i = 3; i > 0; i--)
		{
			userinterface::recordCountDown = i;
			printf("%i\n", i);
			timing::Wait(SECOND);
		}
		userinterface::recordCountDown = 0;
		printf("0\n");

		recordings.push_back(Recording(std::format("Recording %i", recordings.size())));
		currentRecording = recordings.at(recordings.size() - 1);

		recordedCmds = 0;
		isRecording = true;
		global::wantsRecord = false;
	}

	void EndRecording()
	{
		isRecording = false;
	}
}
