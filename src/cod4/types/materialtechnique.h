#pragma once
#include "materialpass.h"
struct MaterialTechnique
{
	const char* name;
	unsigned __int16 flags;
	unsigned __int16 passCount;
	MaterialPass passArray[1];	// count = passCount
};