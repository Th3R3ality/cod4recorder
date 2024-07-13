#pragma once
struct FxElemDef;

struct FxEffectDef
{
	const char* name;
	int flags;
	int totalSize;
	int msecLoopingLife;
	int elemDefCountLooping;
	int elemDefCountOneShot;
	int elemDefCountEmission;
	FxElemDef* elemDefs;
};