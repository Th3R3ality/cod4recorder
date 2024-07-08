#pragma once
#include "../enums/maptype.h"
#include "gfxtexture.h"
#include "picmip.h"
#include "cardmemory.h"
struct GfxImage
{
	MapType mapType;
	GfxTexture texture;
	Picmip picmip;
	bool noPicmip;
	char semantic;
	char track;
	CardMemory cardMemory;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 depth;
	char category;
	bool delayLoadPixels;
	const char* name;
};