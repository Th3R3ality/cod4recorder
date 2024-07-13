#include "hk_decl.h"

namespace hooks
{
	__declspec(naked) void antihook()
	{
		__asm ret
	}
}