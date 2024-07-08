#pragma once
#include "vec.h"
struct MaterialConstantDef
{
	int nameHash;
	char name[12];
	vec4_t literal;
};