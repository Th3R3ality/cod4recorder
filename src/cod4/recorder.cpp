#include "recorder.h"

#include <iostream>
#include <format>

#include "../timing.h"
#include "../userinterface.h"

namespace recorder
{
	void NewRecording()
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

		recordings.push_back(Recording(std::format("Recording {}", recordings.size()+1)));
		currentRecordingIndex = recordings.size() - 1;

		recordedCmds = 0;
		isRecording = true;
		global::wantsRecord = false;
	}

	void StopRecording()
	{
		currentRecordingIndex = -1;
		isRecording = false;
	}
}
