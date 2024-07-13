#pragma once
#include "gfxcolor.h"
#include "packedtexcoords.h"
#include "packedunitvec.h"

struct GfxPackedVertex
{
	float xyz[3];
	float binormalSign;
	GfxColor color;
	PackedTexCoords texCoord;
	PackedUnitVec normal;
	PackedUnitVec tangent;
};