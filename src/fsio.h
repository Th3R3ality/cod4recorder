#pragma once
#include <vector>

#include "cod4/recorder.h"

namespace fsio // filesystem I/O
{
	struct DiskReplay
	{
		char name[sizeof(recorder::Recording::name)] = {};
		unsigned long long uuid = 0;
		fvec3 startPos = {0.0f, 0.0f, 0.0f};
		fvec2 startRot = {0.0f, 0.0f};
		size_t cmdCount = 0;
		size_t fragmentCmdCap = 0;
		size_t offset = 0;
		bool inUse = false;

		DiskReplay() = default;

		DiskReplay(long long uuid)
			: uuid(uuid)
		{}

		DiskReplay(std::shared_ptr<recorder::Recording> recording)
		{
			strncpy_s<sizeof(DiskReplay::name)>(this->name, recording->name, sizeof(DiskReplay::name) - 1);
			this->name[sizeof(DiskReplay::name) - 1] = '\0';
			uuid = recording->uuid;
			startPos = recording->startPos;
			startRot = recording->startRot;
			fragmentCmdCap = cmdCount = recording->cmds.size();

			//size_t offset = 0;	These are set in fsio.cpp
			//bool inUse = false;
		}
	};

	enum SaveError
	{
		None = 0,
		NotInitialized,
		RecordingDoesntExist,
		IndexOutOfRange,
		RecordingAlreadyOnDisk,
		RecordingNotOnDisk,
		BadFile,
		RecordingAlreadyInMemory,
	};

	void Init();
	std::vector<DiskReplay>& PeekSavedRecordings();
	SaveError SaveRecordingToDisk(std::shared_ptr<recorder::Recording> recording);
	SaveError DeleteRecordingOnDisk(unsigned long long uuid);
	SaveError LoadRecordingFromDisk(unsigned long long uuid);
	const char* GetErrorMessage(SaveError error);

	void RecordingWasRemovedFromMemory(unsigned long long uuid);
}