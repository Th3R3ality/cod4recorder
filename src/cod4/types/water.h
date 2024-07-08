#pragma once
#include "waterwritable.h"
#include "complex.h"
#include "gfximage.h"
struct water_t
{
	WaterWritable writable;
	complex_t* H0;
	float* wTerm;
	int M;
	int N;
	float Lx;
	float Lz;
	float gravity;
	float windvel;
	float winddir[2];
	float amplitude;
	float codeConstant[4];
	GfxImage* image;
};