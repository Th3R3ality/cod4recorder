#pragma once
#include "savefile.h"

#ifdef _DEBUG
#define LOG_SAVEFILE_RETURNS
#endif

#ifdef LOG_SAVEFILE_RETURNS
#define LOG_RET(err) \
{\
	printf("%s : %s\n", __func__, savefile::GetErrorMessage(err));\
	return err; \
}
#else
#define LOG_RET(err) \
return err;
#endif