#include "initcod4.h"
#include "globaldatapointers.h"
#include "cod4x.h"

namespace cod4
{
	void Init()
	{
		if (uintptr_t base = cod4x::base())
		{
			dataptr::BG_WeaponDefs = reinterpret_cast<WeaponDef**>(base + 0x443DDE0);

		}

	}
}