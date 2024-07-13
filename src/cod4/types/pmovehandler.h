#pragma once
struct pmoveHandler_t
{
	void(__cdecl* trace)(struct trace_t*, const float*, const float*, const float*, const float*, int, int);
	void(__cdecl* playerEvent)(int, int);
};