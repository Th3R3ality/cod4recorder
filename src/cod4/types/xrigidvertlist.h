#pragma once
#include "xsurfacecollisiontree.h"

struct XRigidVertList
{
	unsigned short boneOffset;
	unsigned short vertCount;
	unsigned short triOffset;
	unsigned short triCount;
	XSurfaceCollisionTree* collisionTree;
};