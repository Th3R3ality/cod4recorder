#pragma once
#include "gfxdrawsurf.h"
#pragma pack(push, 4)
struct MaterialInfo
{
	const char* name;
	char gameFlags;
	char sortKey;
	char textureAtlasRowCount;
	char textureAtlasColumnCount;
	GfxDrawSurf drawSurf;
	unsigned int surfaceTypeBits;
	unsigned __int16 hashIndex;
};
#pragma pack(pop)