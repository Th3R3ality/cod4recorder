#pragma once
#include <vector>
#include <array>
#include <string>
#include <random>
#include "types/usercmd.h"

namespace recorder
{
	struct Smallcmd
	{
		int servertime = 0;
		int buttons = 0;
		std::array<int, 2> viewangles = {0, 0};
		char forwardmove = 0;
		char sidemove = 0;
		unsigned short nextFps = 0;

		Smallcmd() = default;
		Smallcmd(const Smallcmd&) = default;
		Smallcmd(Smallcmd&&) = default;

		explicit Smallcmd(const usercmd_t* cmd) :
			servertime(cmd->servertime),
			buttons(cmd->buttons),
			forwardmove(cmd->forwardmove),
			sidemove(cmd->sidemove)
		{
			viewangles[0] = cmd->viewangles[0];
			viewangles[1] = cmd->viewangles[1];
		}
	};
	
	struct Recording
	{
		char name[64] = {};
		unsigned long long uuid;
		std::vector<Smallcmd> cmds = {};
		size_t onDiskOffset = 0;
		bool onDisk = false;

		Recording(std::string name, unsigned long long uuid)
			: uuid(uuid)
		{
			strncpy_s<sizeof(this->name)>(this->name, name.data(), sizeof(this->name)-1);
			this->name[sizeof(this->name) - 1] = '\0';
		}

		explicit(false) Recording(std::string name)
			: uuid(0)
		{
			strncpy_s<sizeof(this->name)>(this->name, name.data(), sizeof(this->name) - 1);
			this->name[sizeof(this->name) - 1] = '\0';

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
	void RecordingWasRemovedFromDisk(unsigned long long uuid);
	void CaptureCommand(const usercmd_t* cmd);
}