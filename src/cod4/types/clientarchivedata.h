#pragma once
struct ClientArchiveData
{
	int serverTime;
	float origin[3];
	float velocity[3];
	int bobCycle;
	int movementDir;
	float viewangles[3];
};