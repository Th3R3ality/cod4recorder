#pragma once
#include "global.h"
#include "userinterface.h"

namespace timing
{
#define SECOND -10000000LL

	inline bool initialized = false;
	void Init();
	void Wait(long long time);
}