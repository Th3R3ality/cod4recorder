#pragma once
#include "playerstate.h"
#include "entitystate.h"
#include "clientstate.h"

struct snapshot_t
{
	int snapFlags;
	int ping;
	int serverTime;
	playerState_t ps;
	int numEntities;
	int numClients;
	entityState_t entities[512];
	clientState_t clients[64];
	int serverCommandSequence;
};