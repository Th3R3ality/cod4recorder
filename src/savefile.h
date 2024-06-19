#pragma once
#include <vector>

#include "cod4/recorder.h"

namespace savefile
{
	struct WeakReplay
	{
		char name[32];
		size_t offset;
		size_t cmdCount;
		bool inUse;
	};

	enum SaveError
	{
		None = 0,
		NotInitialized,
		RecordingDoesntExist,
	};

	void Init();
	std::vector<WeakReplay>& PeekSavedRecordings();
	SaveError SaveRecording(recorder::Recording& recording);
	SaveError DeleteRecording(std::string name);
}