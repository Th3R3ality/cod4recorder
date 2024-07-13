#pragma once
#include "xsurfacecollisionnode.h"
#include "xsurfacecollisionleaf.h"

struct XSurfaceCollisionTree
{
	float trans[3];
	float scale[3];
	unsigned int nodeCount;
	XSurfaceCollisionNode* nodes;
	unsigned int leafCount;
	XSurfaceCollisionLeaf* leafs;
};