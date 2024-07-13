#pragma once
#include "gfxdepthoffield.h"
#include "gfxfilm.h"
#include "gfxglow.h"
#include "gfxlight.h"
#include "gfxviewport.h"

struct refdef_t
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	float tanHalfFovX;
	float tanHalfFovY;
	fvec3 vieworg;
	fvec3 viewaxis[3];
	fvec3 viewOffset;
	int time;
	float zNear;
	float blurRadius;
	GfxDepthOfField dof;
	GfxFilm film;
	GfxGlow glow;
	GfxLight primaryLights[255];
	GfxViewport scissorViewport;
	bool useScissorViewport;
	int localClientNum;
};