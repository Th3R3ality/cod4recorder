#pragma once
#include "gfxdrawsurffields.h"
union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned long long packed;
};