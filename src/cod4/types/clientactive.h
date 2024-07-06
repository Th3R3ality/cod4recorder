#pragma once
#include "clsnapshot.h"
#include "gamestate.h"
#include "../enums/stancestate.h"
#include "usercmd.h"
#include "clientarchivedata.h"
#include "outpacket.h"
#include "entitystate.h"
#include "clientstate.h"

struct clientActive_t
{
	bool usingAds;
	int timeoutcount;
	clSnapshot_t snap;
	bool alwaysFalse;
	int serverTime;
	int oldServerTime;			// to prevent time from flowing bakcwards
	int oldFrameServerTime;		// to check tournament restarts
	int serverTimeDelta;		// cl.serverTime = cls.realtime + cl.serverTimeDelta
	int oldSnapServerTime;
	int extrapolatedSnapshot;	// set if any cgame frame has been forced to extrapolate
	int newSnapshots;			// set on parse of any valid packet
	gameState_t gameState;		// configstrings
	char mapname[64];			// extracted from CS_SERVERINFO
	int parseEntitiesNum;		// index (not anded off) into cl_parse_entities[]
	int parseClientsNum;
	int mouseDx[2];				// added to by mouse events
	int mouseDy[2];				// added to by mouse events
	int mouseIndex;
	bool stanceHeld;
	StanceState stance;
	StanceState stancePosition;
	int stanceTime;
	int cgameUserCmdWeapon;
	int cgameUserCmdOffHandIndex;
	float cgameFOVSensitivityScale;
	float cgameMaxPitchSpeed;
	float cgameMaxYawSpeed;
	float cgameKickAngles[3];
	float cgameOrigin[3];
	float cgameVelocity[3];
	float cgameViewangles[3];
	int cgameBobCycle;
	int cgameMovementDir;
	int cgameExtraButtons;
	int cgamePredictedDataServerTime;
	float viewangles[3];
	int serverId;
	int skelTimeStamp;
	volatile int skelMemPos;
	char skelMemory[262144];
	char* skelMemoryStart;
	bool allowedAllocSkel;

	// cmds[cmdNumber] is the predicted command, [cmdNumber-1] is the last
	// properly generated command
	__declspec(align(4)) usercmd_t cmds[128];
	int cmdNumber;
	ClientArchiveData clientArchive[256];
	int clientArchiveIndex;
	outPacket_t outPackets[32];
	clSnapshot_t snapshots[32];
	entityState_t entityBaselines[1024]; // for delta compression when not in previous frame
	entityState_t parseEntities[2048];
	clientState_t parseClients[2048];
	int corruptedTranslationFile;
	char translationVersion[256];
	float vehicleViewYaw;
	float vehicleViewPitch;
};