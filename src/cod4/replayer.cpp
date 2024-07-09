#include "replayer.h"

#include "../global.h"
#include "../userinterface.h"
#include "../timing.h"
#include "../helper.h"
#include "dataid.h"
#include "angles.h"

namespace replayer
{
	bool wantsToReplay = false;
	std::shared_ptr<recorder::Recording> currentRecording = nullptr;

	void PlaySelectedRecording(bool autoHideMenu, bool skipDelay)
	{
		currentRecording = recorder::currentRecording;

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

		wantsToReplay = false;
		isReplaying = true;
	}
	void Stop(usercmd_t* const cmd)
	{
		currentRecording = nullptr;
		isReplaying = false;

		fvec2 deltaRot = GetViewDeltaToDirection(GetViewAngles()); //dataptr::client->cgameViewangles

		dataptr::client->viewangles[0] += deltaRot.x;
		dataptr::client->viewangles[1] += deltaRot.y;

		if (cmd != nullptr)
		{
			cmd->viewangles[0] += ANGLE2SHORT(deltaRot.x);
			cmd->viewangles[1] += ANGLE2SHORT(deltaRot.y);

			if (recorder::WantsToRecord())
			{
				recorder::StartRecordingSegment(cmdIndex, replayer::startServertime);
				recorder::CaptureCommand(cmd);
			}
		}

		replayer::startServertime = 0;
		cmdIndex = 0;
	}

	void WantReplay(bool repeating)
	{
		wantsToReplay = true;
		autoReplay = repeating;
	}

	bool WantsToReplay()
	{
		return wantsToReplay || (autoReplay && !isReplaying);
	}

	void ModifyNextCommand(usercmd_t* const cmd)
	{
		if (currentRecording == nullptr)
			return;

		static dvar_t* const dvar_maxfps = GetDvar(dataid::dvar::com_maxfps.offset);
		static int viewangle0offset = 0;
		static int viewangle1offset = 0;

		if (cmd->forwardmove != 0 || cmd->sidemove != 0)
		{
			return replayer::Stop(cmd);
		}
		if (replayer::cmdIndex >= currentRecording->cmds.size())
		{
			return replayer::Stop(cmd);
		}

		auto& smallcmd = currentRecording->cmds.at(replayer::cmdIndex);
		if (replayer::cmdIndex == 0)
		{
			startServertime = cmd->servertime;

			fvec2 deltaRot = GetViewDeltaToDirection(currentRecording->startRot);

			dataptr::client->viewangles[0] += deltaRot.x;
			dataptr::client->viewangles[1] += deltaRot.y;

			cmd->viewangles[0] += ANGLE2SHORT(deltaRot.x);
			cmd->viewangles[1] += ANGLE2SHORT(deltaRot.y);

			viewangle0offset = smallcmd.viewangles[0] - cmd->viewangles[0];
			viewangle1offset = smallcmd.viewangles[1] - cmd->viewangles[1];
		}

		if (smallcmd.nextFps != 0)
			dvar_maxfps->current.integer = smallcmd.nextFps;

		cmd->servertime = startServertime + smallcmd.servertime;
		cmd->buttons = smallcmd.buttons;

		//cmd->viewangles[0] = smallcmd.viewangles[0] - viewangle0offset;
		//cmd->viewangles[1] = smallcmd.viewangles[1] - viewangle1offset;
		ivec2 deltaRot = GetViewDeltaToDirection(ivec2(smallcmd.viewangles.data()));

		cmd->viewangles[0] += deltaRot.x;
		cmd->viewangles[1] += deltaRot.y;

		cmd->forwardmove = smallcmd.forwardmove;
		cmd->sidemove = smallcmd.sidemove;

		replayer::cmdIndex++;
	}
}
