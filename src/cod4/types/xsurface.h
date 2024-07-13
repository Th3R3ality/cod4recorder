#pragma once
#include "xsurfacevertexinfo.h"
#include "gfxpackedvertex.h"
#include "xrigidvertlist.h"

struct XSurface
{
	char tileMode;
	bool deformed;
	unsigned __int16 vertCount;
	unsigned __int16 triCount;
	char zoneHandle;
	unsigned __int16 baseTriIndex;
	unsigned __int16 baseVertIndex;
	unsigned __int16* triIndices;
	XSurfaceVertexInfo vertInfo;
	GfxPackedVertex* verts0;
	unsigned int vertListCount;
	XRigidVertList* vertList;
	int partBits[4];
};