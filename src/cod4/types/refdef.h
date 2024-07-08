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
	float vieworg[3];
	float viewaxis[3][3];
	float viewOffset[3];
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