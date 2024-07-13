#pragma once
#pragma pack(push, 4)
struct PhysPreset
{
	const char* name;
	int type;
	float mass;
	float bounce;
	float friction;
	float bulletForceScale;
	float explosiveForceScale;
	const char* sndAliasPrefix;
	float piecesSpreadFraction;
	float piecesUpwardVelocity;
	char tempDefaultToCylinder;
};
#pragma pack(pop)