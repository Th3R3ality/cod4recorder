#pragma once
#include "gfxskincacheentry.h"
struct cpose_t
{
	unsigned __int16 lightingHandle;
	char eType;
	char eTypeUnion;
	char localClientNum;
	int cullIn;
	char isRagdoll;
	int ragdollHandle;
	int killcamRagdollHandle;
	int physObjId;
	float origin[3];
	float angles[3];
	GfxSkinCacheEntry skinCacheEntry;
	//$EAE81CC4C8A7A2F7E95AA09AC9F9F9C0 ___u12;
	char pad[0x24];
};