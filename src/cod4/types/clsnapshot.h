#pragma once
#include "playerState.h"

struct clSnapshot_t
{
	int valid;
	int snapFlags;// SNAPFLAG_RATE_DELAYED, etc
	int serverTime; // server time the message is valid for (in msec)
	int messageNum;
	int deltaNum;
	int ping;
	int cmdNum;
	playerState_t ps; // complete information about the current player at this time
	int numEntities;
	int numClients;
	int parseEntitiesNum;
	int parseClientsNum;
	int serverCommandNum; //usercmd count
};