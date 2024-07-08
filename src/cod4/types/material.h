#pragma once
#include "materialinfo.h"
#include "materialtechniqueset.h"
#include "materialtexturedef.h"
#include "materialconstantdef.h"
#include "gfxstatebits.h"

struct Material
{
	MaterialInfo info;
	char stateBitsEntry[34];
	char textureCount;
	char constantCount;
	char stateBitsCount;
	char stateFlags;
	char cameraRegion;
	MaterialTechniqueSet* techniqueSet;
	MaterialTextureDef* textureTable;
	MaterialConstantDef* constantTable;
	GfxStateBits* stateBitsTable;
};