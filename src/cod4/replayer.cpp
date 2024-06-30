#include "replayer.h"

#include "../global.h"
#include "../userinterface.h"
#include "../timing.h"
#include "../helper.h"
#include "dataid.h"

namespace replayer
{
	recorder::Recording* currentRecording = nullptr;

	void PlaySelectedRecording(bool autoHideMenu, bool skipDelay)
	{
		currentRecording = &recorder::recordings.at(selectedRecordingIndex);

		if (!skipDelay)
		{
			printf("Waiting 3 Seconds before playing...\n");

			for (int i = 3; i > 0; i--)
			{
				userinterface::replayCountDown = i;
				printf("%i\n", i);
				timing::Wait(SECOND);
			}
			userinterface::replayCountDown = 0;
			printf("0\n");
		}
		printf("replaying\n");

		if (autoHideMenu)
			userinterface::showMenu = false;

		isReplaying = true;
	}
	void SelectRecordingByIndex(int index)
	{
		if (!(index > -1 && index < recorder::recordings.size()))
			return;

		selectedRecordingIndex = index;
	}
	void Stop()
	{
		currentRecording = nullptr;
		recordingIndex = 0;
		isReplaying = false;
	}

	void ModifyNextCommand(usercmd_t* const cmd)
	{
		if (currentRecording == nullptr)
			return;

		static dvar_t* const dvar_maxfps = GetDvar(dvarids::com_maxfps.offset);
		static int startServertime = 0;
		static int viewangle0offset = 0;
		static int viewangle1offset = 0;

		if (cmd->forwardmove != 0 || cmd->sidemove != 0)
		{
			return replayer::Stop();
		}

		if (replayer::recordingIndex >= currentRecording->cmds.size())
		{
			return replayer::Stop();
		}
		auto& smallcmd = currentRecording->cmds.at(replayer::recordingIndex);

		if (replayer::recordingIndex == 0)
		{
			startServertime = cmd->servertime;
			viewangle0offset = smallcmd.viewangles[0] - cmd->viewangles[0];
			viewangle1offset = smallcmd.viewangles[1] - cmd->viewangles[1];
		}

		if (smallcmd.nextFps != 0)
			dvar_maxfps->current.integer = smallcmd.nextFps;

		cmd->servertime = startServertime + smallcmd.servertime;
		cmd->buttons = smallcmd.buttons;

		cmd->viewangles[0] = smallcmd.viewangles[0] - viewangle0offset;
		cmd->viewangles[1] = smallcmd.viewangles[1] - viewangle1offset;

		cmd->forwardmove = smallcmd.forwardmove;
		cmd->sidemove = smallcmd.sidemove;

		replayer::recordingIndex++;
	}
}
