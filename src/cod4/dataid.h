#pragma once
#include <string>

struct SigOffsetPair
{
	const char* sig;
	unsigned short sigStrSize;
	unsigned short sigOffset;
	unsigned long offset;
};

#define SIGWITHSIZE(x) x, sizeof(x)

namespace funcids
{
	constexpr SigOffsetPair CL_Frame = { SIGWITHSIZE("51 80 3D ?? ?? ?? ?? ?? 56 57 8B 3D ?? ?? ?? ?? 8B"), 0, 0x6C9A0 };

	constexpr SigOffsetPair CL_CreateCmd = { SIGWITHSIZE("83 EC 10 D9 05 ?? ?? ?? ?? 57"), 0, 0x63D10};
	constexpr SigOffsetPair CL_CmdButtons = { SIGWITHSIZE("69 C0 58 02 00 00 8D"), 0, 0x63860 };
	constexpr SigOffsetPair CL_KeyMove = { SIGWITHSIZE("51 53 56 8B F0 69"), 0, 0x631B0 };
	constexpr SigOffsetPair CL_MouseMove = { SIGWITHSIZE("83 EC 4C 53 8B 5C 24 54 56"), 0, 0x63490 };
	constexpr SigOffsetPair CL_FinishMove = { SIGWITHSIZE("A0 ?? ?? ?? ?? 8A 0D"), 0, 0x63A60 };

}

namespace dvarids
{
	constexpr SigOffsetPair com_maxfps = { SIGWITHSIZE(""), 0, 0x1076EF8 };
}

#undef SIGWITHSIZE