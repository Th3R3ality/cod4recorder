#include "timing.h"
#include <iostream>

namespace timing
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	void Init()
	{
		hTimer = CreateWaitableTimerA(NULL, TRUE, NULL);
		if (NULL == hTimer)
		{
			printf("CreateWaitableTimer failed (%d)\n", GetLastError());
			return;
		}
		initialized = true;
	}

	void Wait(long long time)
	{
		liDueTime.QuadPart = time;
		if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
		{
			printf("SetWaitableTimer failed (%d)\n", GetLastError()); return;
		}

		if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
		{
			printf("WaitForSingleObject failed (%d)\n", GetLastError());
		}
	}
}