#pragma once
#include "dobjanimmat.h"
#include "xsurface.h"
#include "material.h"
#include "xmodellodinfo.h"
#include "xmodelcollsurf.h"
#include "xboneinfo.h"
#include "xmodelstreaminfo.h"
#include "physpreset.h"
#include "physgeomlist.h"

struct XModel
{
	const char* name;
	char numBones;
	char numRootBones;
	unsigned char numsurfs;
	char lodRampType;
	unsigned __int16* boneNames;
	char* parentList;
	__int16* quats;
	float* trans;
	char* partClassification;
	DObjAnimMat* baseMat;
	XSurface* surfs;
	Material** materialHandles;
	XModelLodInfo lodInfo[4];
	XModelCollSurf_t* collSurfs;
	int numCollSurfs;
	int contents;
	XBoneInfo* boneInfo;
	float radius;
	float mins[3];
	float maxs[3];
	__int16 numLods;
	__int16 collLod;
	XModelStreamInfo streamInfo;
	int memUsage;
	char flags;
	bool bad;
	PhysPreset* physPreset;
	PhysGeomList* physGeoms;
};