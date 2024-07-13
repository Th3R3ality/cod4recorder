#pragma once
#include "../../vec.h"

struct usercmd_t
{
	int servertime;
	int buttons;
	ivec3 viewangles;
	char weaponIndex;
	char a1;
	char forwardmove;
	char sidemove;
	int i1;
	int i2;
};