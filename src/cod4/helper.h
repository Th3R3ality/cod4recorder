#pragma once
#include "types/include.h"
#include "../global.h"

inline dvar_t* GetDvar(unsigned long DvarOffset)
{
	return *(dvar_t**)(global::baseAddress + DvarOffset);
}