#pragma once
#include <string>

namespace dataid
{
	struct SigOffsetPair
	{
		const char* sig;
		unsigned short sigStrSize;
		unsigned short sigOffset;
		unsigned long offset;
		unsigned long addr;
	};

#define SIGWITHSIZE(x) x, sizeof(x)
#define BASE 0x400000

	namespace func
	{
		constexpr SigOffsetPair CL_Frame = { SIGWITHSIZE("51 80 3D ?? ?? ?? ?? ?? 56 57 8B 3D ?? ?? ?? ?? 8B"), 0, 0x6C9A0, 0x6C9A0 + BASE };

		constexpr SigOffsetPair CL_CreateCmd = { SIGWITHSIZE("83 EC 10 D9 05 ?? ?? ?? ?? 57"), 0, 0x63D10, 0x63D10 + BASE};
		constexpr SigOffsetPair CL_CmdButtons = { SIGWITHSIZE("69 C0 58 02 00 00 8D"), 0, 0x63860 , 0x63860 + BASE};
		constexpr SigOffsetPair CL_KeyMove = { SIGWITHSIZE("51 53 56 8B F0 69"), 0, 0x631B0 , 0x631B0 + BASE};
		constexpr SigOffsetPair CL_MouseMove = { SIGWITHSIZE("83 EC 4C 53 8B 5C 24 54 56"), 0, 0x63490 , 0x63490 + BASE};
		constexpr SigOffsetPair CL_FinishMove = { SIGWITHSIZE("A0 ?? ?? ?? ?? 8A 0D"), 0, 0x63A60 , 0x63A60 + BASE};
		constexpr SigOffsetPair restart_input = { SIGWITHSIZE(""), 0, 0x175E90 , 0x175E90 + BASE};
	}

	namespace d3d
	{
		constexpr SigOffsetPair device = { SIGWITHSIZE(""), 0, 0xC89A408 , 0xC89A408 + BASE};
	}

	namespace dvar
	{
		constexpr SigOffsetPair com_maxfps = { SIGWITHSIZE(""), 0, 0x1076EF8 , 0x1076EF8 + BASE};
		constexpr SigOffsetPair cl_ingame = { SIGWITHSIZE(""), 0, 0x85F8F0 , 0x85F8F0 + BASE};
		constexpr SigOffsetPair cl_paused = { SIGWITHSIZE(""), 0, 0x1035D48 , 0x1035D48 + BASE};
		constexpr SigOffsetPair sv_paused = { SIGWITHSIZE(""), 0, 0x1076ED4 , 0x1076ED4 + BASE};
		constexpr SigOffsetPair sv_running = { SIGWITHSIZE(""), 0, 0x1035D60 , 0x1035D60 + BASE};
		constexpr SigOffsetPair in_mouse = { SIGWITHSIZE(""), 0, 0xC8120E4, 0xC8120E4 + BASE};
	}

	namespace boolean
	{
		constexpr SigOffsetPair mouse_enabled = { SIGWITHSIZE(""), 0, 0xC8147D1, 0xC8147D1 + BASE };
	}
#undef BASE
#undef SIGWITHSIZE
}