#pragma once
#include "materialvertexstreamrouting.h"
struct MaterialVertexDeclaration
{
	char streamCount;
	bool hasOptionalSource;
	bool isLoaded;
	MaterialVertexStreamRouting routing;
};