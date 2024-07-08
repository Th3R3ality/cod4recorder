#pragma once
#include "gfximage.h"
struct __declspec(align(4)) GfxLightImage
{
	GfxImage* image;
	char samplerState;
};
