#pragma once
struct XModelLodInfo
{
	float dist;
	unsigned __int16 numsurfs;
	unsigned __int16 surfIndex;
	int partBits[4];
	char lod;
	char smcIndexPlusOne;
	char smcAllocBits;
	char unused;
};