#pragma once
#include <vector>

#include "cod4/recorder.h"

namespace savefile
{
	struct DiskReplay
	{
		char name[64] = {};
		unsigned long long uuid = 0;
		size_t offset = 0;
		size_t cmdCount = 0;
		bool inUse = false;

		DiskReplay() = default;

		DiskReplay(long long uuid)
			: uuid(uuid)
		{}
	};

	enum SaveError
	{
		None = 0,
		NotInitialized,
		RecordingDoesntExist,
		IndexOutOfRange,
		RecordingAlreadyOnDisk,
		RecordingNotOnDisk,
	};

	void Init();
	std::vector<DiskReplay>& PeekSavedRecordings();
	SaveError SaveRecordingToDisk(int recordingIndex);
	SaveError DeleteRecordingOnDisk(unsigned long long uuid);
	SaveError LoadRecordingFromDisk(unsigned long long uuid);
}