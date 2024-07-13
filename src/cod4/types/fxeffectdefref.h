#pragma once
#include "fxeffectdef.h"

union FxEffectDefRef
{
	FxEffectDef* handle;
	const char* name;
};