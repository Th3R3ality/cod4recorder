#pragma once
struct shellshock_t
{
	void* parms; //shellshock_parms_t
	int startTime;
	int duration;
	int loopEndTime;
	float sensitivity;
	float viewDelta[2];
	int hasSavedScreen;
};