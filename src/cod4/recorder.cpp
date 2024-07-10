#include "recorder.h"

#include <iostream>
#include <format>

#include "../timing.h"
#include "../userinterface.h"
#include "dataid.h"
#include "../helper.h"
#include "angles.h"

namespace recorder
{
	bool wantsToRecord = false;
	bool isNewRecording = true;

	int startServertime = 0;
	unsigned short lastFps = 0;
	//bool isNewSegment = true;

	void NewRecording()
	{
		//printf("Waiting 3 Seconds before recording...\n");
		//for (int i = 3; i > 0; i--)
		//{
		//	userinterface::recordCountDown = i;
		//	printf("%i\n", i);
		//	timing::Wait(SECOND);
		//}
		//userinterface::recordCountDown = 0;
		//printf("0\n");

		//_recordings.emplace_back(std::format("Recording {}", _recordings.size()+1));
		//currentRecordingIndex = _recordings.size() - 1;
		//currentRecording = &_recordings.at(currentRecordingIndex);

		currentRecording = std::make_shared<Recording>("New Recording");
		recordings.push_back(currentRecording);
		currentSegment.clear();
		currentSegmentStartIndex = 0;
		//isRecording = true;
		//global::wantsRecord = false;
	}

	void StartRecordingSegment(unsigned int segmentStartIndex, int recordingStartServertime)
	{
		currentSegment.clear();
		currentSegmentStartIndex = segmentStartIndex;

		if (segmentStartIndex == 0)
		{
			currentRecording->cmds.clear();
			startServertime = 0;
			isNewRecording = true;
			waitingForStandstill = true;
			waitingForMove = false;
		}

		if (recordingStartServertime != 0)
			startServertime = recordingStartServertime;

		lastFps = 0;
		wantsToRecord = false;
		userinterface::showMenu = false;
		isRecording = true;
		
	}

	void StopRecordingSegment()
	{
		//currentRecording = nullptr;
		//currentRecordingIndex = -1;
		waitingForStandstill = false;
		waitingForMove = false;
		isRecording = false;
	}

	void SaveSegmentToCurrentRecording()
	{
		currentRecording->cmds.resize(currentSegmentStartIndex + currentSegment.size());
		std::copy(currentSegment.begin(), currentSegment.end(), currentRecording->cmds.begin() + currentSegmentStartIndex);

		DiscardSegment();
	}

	void DiscardSegment()
	{
		currentSegment.clear();
		currentSegmentStartIndex = 0;
	}

	void WantNewSegment()
	{
		if (currentRecording->cmds.size() == 0)
			isNewRecording = true;
		else
		{
			replayer::WantReplay();
		}
		wantsToRecord = true;
	}

	bool WantsToRecord()
	{
		return wantsToRecord;
	}

	bool CanStartRecording()
	{
		return (!recorder::isRecording && isNewRecording && !replayer::isReplaying);
	}

	void CaptureCommand(usercmd_t* const cmd)
	{
		static const dvar_t* dvar_maxfps = GetDvar(dataid::dvar::com_maxfps.offset);

		if (isNewRecording)
		{
			if (waitingForStandstill) // nesting these for performance (only 1 if check per iteration)
			{
				if (dataptr::client->cgameVelocity.MagSq() != 0)
					return;
				else
				{
					waitingForStandstill = false;
					waitingForMove = true;
				}
			}
			if (waitingForMove) // nesting these for performance (only 1 if check per iteration)
			{
				if (cmd->forwardmove || cmd->sidemove)
				{
					cmd->forwardmove = cmd->sidemove = 0;
					waitingForMove = false;
				}
				
				return;
			}

			currentRecording->startRot = GetRealAngles();
			currentRecording->startPos = dataptr::client->cgameOrigin;

			startServertime = cmd->servertime;
			isNewRecording = false;
		}

		auto scmd = Smallcmd(cmd, GetRealCmdAngles(cmd));
		scmd.servertime -= startServertime;
		currentSegment.push_back(scmd);

		if (global::debugPrints)
			printf("captured cmd\n");

		if (currentSegment.size() > 1)
		{
			auto currentFps = static_cast<unsigned short>(dvar_maxfps->current.integer);
			if (currentFps != lastFps)
			{
				currentSegment.at(currentSegment.size() - 2).nextFps = currentFps;
			}
			lastFps = currentFps;
		}
	}

	void RecordingWasRemovedFromDisk(unsigned long long uuid)
	{
		for (auto& recording : recordings)
		{
			if (recording->uuid != uuid)
				continue;

			recording->onDisk = false;
			recording->onDiskOffset = 0;
			break;
		}
	}
	bool IsRecordingInMemory(unsigned long long uuid)
	{
		for (auto& recording : recordings)
		{
			if (recording->uuid == uuid)
				return true;
		}
		return false;
	}
}
