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

	int startServertime = 0;
	unsigned short lastFps = 0;
	//bool isNewSegment = true;
	bool isNewRecording = true;

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
		}

		if (recordingStartServertime != 0)
			startServertime = recordingStartServertime;

		lastFps = 0;
		wantsToRecord = false;
		isRecording = true;
	}

	void StopRecordingSegment()
	{
		//currentRecording = nullptr;
		//currentRecordingIndex = -1;
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

	void WantNewSegment(bool isFirstSegment)
	{
		if (isFirstSegment)
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

	void CaptureCommand(const usercmd_t* const cmd)
	{
		static const dvar_t* dvar_maxfps = GetDvar(dataid::dvar::com_maxfps.offset);

		if (isNewRecording)
		{
			currentRecording->startRot = GetRealAngles();
			currentRecording->startPos = fvec3(dataptr::client->cgameOrigin);

			startServertime = cmd->servertime;
			isNewRecording = false;
		}

		auto scmd = Smallcmd(cmd);
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
}
