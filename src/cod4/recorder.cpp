#include "recorder.h"

#include <iostream>
#include <format>

#include "../timing.h"
#include "../userinterface.h"
#include "dataid.h"
#include "../helper.h"

namespace recorder
{
	Recording* currentRecording = nullptr;

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

		recordings.emplace_back(std::format("Recording {}", recordings.size()+1));
		currentRecordingIndex = recordings.size() - 1;
		currentRecording = &recordings.at(currentRecordingIndex);

		recordedCmds = 0;
		isRecording = true;
		global::wantsRecord = false;
	}

	void StopRecording()
	{
		currentRecording = nullptr;
		currentRecordingIndex = -1;
		isRecording = false;
	}
	void RecordingWasRemovedFromDisk(unsigned long long uuid)
	{
		for (auto& recording : recordings)
		{
			if (recording.uuid != uuid)
				continue;
			
			recording.onDisk = false;
			recording.onDiskOffset = 0;
			break;
		}
	}
	void CaptureCommand(const usercmd_t* cmd)
	{
		static const dvar_t* dvar_maxfps = GetDvar(dataid::dvar::com_maxfps.offset);
		static int startServertime = 0;
		static unsigned short lastFps = 0;

		if (currentRecording->cmds.empty())
		{
			startServertime = cmd->servertime;
			lastFps = 0;
		}

		auto scmd = Smallcmd(cmd);
		scmd.servertime -= startServertime;

		currentRecording->cmds.push_back(scmd);
		recordedCmds++;

		if (currentRecording->cmds.size() > 1)
		{
			auto currentFps = static_cast<unsigned short>(dvar_maxfps->current.integer);
			if (currentFps != lastFps)
			{
				currentRecording->cmds.at(currentRecording->cmds.size() - 2).nextFps = currentFps;
			}
			lastFps = currentFps;
		}
	}
}
