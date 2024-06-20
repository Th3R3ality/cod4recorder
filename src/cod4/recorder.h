#pragma once
#include <vector>
#include <string>
#include <random>

namespace recorder
{
	struct Smallcmd
	{
		int servertime;
		int buttons;
		int viewangles[2];
		char forwardmove;
		char sidemove;
		unsigned short nextFps;
	};
	
	struct Recording
	{
		std::string name;
		unsigned long long uuid;
		std::vector<Smallcmd> cmds = {};
		size_t onDiskOffset = 0;
		bool onDisk = false;

		Recording(std::string name, unsigned long long uuid)
			: name(name), uuid(uuid)
		{}

		Recording(std::string name)
			: name(name), uuid(0)
		{
			std::random_device rd;
			std::mt19937_64 rng(rd());
			std::uniform_int_distribution<std::mt19937_64::result_type> distribution(0LL);
			uuid = distribution(rng);
		}
	};

	inline unsigned int recordedCmds = 0;
	inline std::vector<Recording> recordings = { };
	inline int currentRecordingIndex = -1;
	inline bool isRecording = false;

	void NewRecording();
	void StopRecording();

}