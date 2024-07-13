#pragma once
#include "fxelemmarkvisuals.h"
#include "fxelemvisuals.h"

union FxElemDefVisuals
{
	FxElemMarkVisuals* markArray;
	FxElemVisuals* array;
	FxElemVisuals instance;
};
