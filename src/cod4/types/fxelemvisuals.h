#pragma once
#include "material.h"
#include "xmodel.h"
#include "fxeffectdefref.h"
union FxElemVisuals
{
	const void* anonymous;
	Material* material;
	XModel* model;
	FxEffectDefRef effectDef;
	const char* soundName;
};