#pragma once
#include "materialargumentcodeconst.h"
union MaterialArgumentDef
{
	const float* literalConst;
	MaterialArgumentCodeConst codeConst;
	unsigned int codeSampler;
	unsigned int nameHash;
};