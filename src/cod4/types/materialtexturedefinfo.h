#pragma once
#include "gfximage.h"
#include "water.h"
union MaterialTextureDefInfo
{
	GfxImage* image;	// MaterialTextureDef->semantic != TS_WATER_MAP
	water_t* water;		// MaterialTextureDef->semantic == TS_WATER_MAP
};