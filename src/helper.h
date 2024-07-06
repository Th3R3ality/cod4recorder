#pragma once
#include "cod4/types/include.h"
#include "global.h"
#include "cod4/recorder.h"
#include "cod4/replayer.h"

inline dvar_t* GetDvar(unsigned long DvarOffset)
{
	return *(dvar_t**)(global::baseAddress + DvarOffset);
}

inline bool CanStartRecording()
{
	return !recorder::isRecording && !replayer::isReplaying;
}

inline bool CanStartReplaying()
{
	return !replayer::isReplaying && !recorder::isRecording;
}

inline float Magnitude(const float vec[3])
{
	return std::sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}