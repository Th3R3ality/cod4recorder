#include "cod4x.h"

namespace cod4x
{
	uintptr_t base()
	{
		static const HMODULE cod4x_handle = GetModuleHandleA("cod4x_021.dll");
		return reinterpret_cast<uintptr_t>(cod4x_handle);
	}

	bool present()
	{
		return !!base();
	}
}
