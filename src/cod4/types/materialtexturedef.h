#pragma once
#include "materialtexturedefinfo.h"
struct MaterialTextureDef
{
	unsigned int nameHash;
	char nameStart;
	char nameEnd;
	char samplerState;
	char semantic;
	MaterialTextureDefInfo u;
};