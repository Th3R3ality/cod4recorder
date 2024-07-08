#pragma once
#include "types/clientactive.h"
#include "types/cg.h"
// base 0x400000

namespace dataptr
{
	inline auto client = (clientActive_t*)0xC5F930;
	inline auto cg = (cg_t*)0x74E338;
}