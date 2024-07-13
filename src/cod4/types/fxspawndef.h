#pragma once
#include "fxspawndeflooping.h"
#include "fxspawndefoneshot.h"

union FxSpawnDef
{
	FxSpawnDefLooping looping;
	FxSpawnDefOneShot oneShot;
};