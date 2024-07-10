#pragma once
#include <vector>
#include <array>
#include <string>
#include <random>
#include <memory>
#include "types/usercmd.h"
#include "../vec.h"

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
		//Smallcmd(const Smallcmd&) = default;
		//Smallcmd(Smallcmd&&) = default;

		explicit Smallcmd(const usercmd_t* cmd) :
			servertime(cmd->servertime),
			buttons(cmd->buttons),
			forwardmove(cmd->forwardmove),
			sidemove(cmd->sidemove)
		{
			viewangles[0] = cmd->viewangles[0];
			viewangles[1] = cmd->viewangles[1];
		}

		explicit Smallcmd(const usercmd_t* cmd, const ivec2& angles) :
			servertime(cmd->servertime),
			buttons(cmd->buttons),
			forwardmove(cmd->forwardmove),
			sidemove(cmd->sidemove)
		{
			viewangles[0] = angles.x;
			viewangles[1] = angles.y;
		}
	};
	
	struct Recording
	{
		char name[64] = {};
		unsigned long long uuid;
		fvec3 startPos = {};
		fvec2 startRot = {};
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

	inline bool isRecording = false;
	inline bool waitingForStandstill = false;
	inline bool waitingForMove = false;

	inline std::vector<std::shared_ptr<Recording>> recordings = {};
	inline std::shared_ptr<Recording> currentRecording = nullptr;

	inline unsigned int currentSegmentStartIndex = 0;
	inline std::vector<Smallcmd> currentSegment = {};

	void NewRecording();
	inline void SelectRecording(std::shared_ptr<Recording> rec) { currentRecording = rec; };
	void StartRecordingSegment(unsigned int segmentStartIndex = 0, int recordingStartServertime = 0);
	void StopRecordingSegment();
	void SaveSegmentToCurrentRecording();
	void DiscardSegment();
	
	void WantNewSegment();
	bool WantsToRecord();
	bool CanStartRecording();

	void CaptureCommand(usercmd_t* const cmd);
	void RecordingWasRemovedFromDisk(unsigned long long uuid);
	bool IsRecordingInMemory(unsigned long long uuid);
}