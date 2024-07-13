#pragma once
#include "xsurfacecollisionaabb.h"

struct XSurfaceCollisionNode
{
	XSurfaceCollisionAabb aabb;
	unsigned short childBeginIndex;
	unsigned short childCount;
};