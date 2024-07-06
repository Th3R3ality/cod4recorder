#pragma once
struct outPacket_t
{
	int p_cmdNumber;		// cl.cmdNumber when packet was sent
	int p_serverTime;		// usercmd->serverTime when packet was sent
	int p_realtime;			// cls.realtime when packet was sent
};