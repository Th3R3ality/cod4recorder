#pragma once
#include "lerpentitystate.h"

// entityState_t is the information conveyed from the server
// in an update message about entities that the client will
// need to render in some way
// Different eTypes may use the information in different ways
// The messages are delta compressed, so it doesn't really matter if
// the structure size is fairly large
struct entityState_t
{
	int number;// entity index
	int eType;// entityType_t
	LerpEntityState lerp;
	int time2;
	int otherEntityNum; // shotgun sources, etc
	int attackerEntityNum;
	int groundEntityNum; // -1 = in air
	int loopSound; // constantly loop this sound
	int surfType;
	int index;
	int clientNum; // 0 to (MAX_CLIENTS - 1), for players and corpses
	int iHeadIcon;
	int iHeadIconTeam;
	int solid; // for client side prediction, trap_linkentity sets this properly
	unsigned int eventParm;
	int eventSequence;
	int events[4]; // impulse events -- muzzle flashes, footsteps, etc
	int eventParms[4];
	int weapon; // determines weapon and flash model, etc
	int weaponModel;
	int legsAnim; // mask off ANIM_TOGGLEBIT
	int torsoAnim; // mask off ANIM_TOGGLEBIT
	int indexUnion1;
	int indexUnion2;
	float fTorsoPitch;
	float fWaistPitch;
	unsigned int partBits[4];
};