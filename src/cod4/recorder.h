#pragma once
#include <vector>
#include <string>

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
		std::vector<Smallcmd> cmds = {};

		Recording(std::string name)
			: name(name)
		{}
	};

	inline unsigned int recordedCmds = 0;
	inline std::vector<Recording> recordings = { Recording("test")};
	inline Recording& currentRecording = recordings.at(0);
	inline bool isRecording = false;

	void BeginRecording();
	void EndRecording();

}