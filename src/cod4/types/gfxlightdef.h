#pragma once
#include "gfxlightimage.h"
struct GfxLightDef
{
	const char* name;
	GfxLightImage attenuation;
	int lmapLookupStart;
};