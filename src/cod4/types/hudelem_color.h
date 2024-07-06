#pragma once
struct hudelem_color_separate_t
{
	char r;
	char g;
	char b;
	char a;
};

union hudelem_color_t
{
	hudelem_color_separate_t __s0;
	int rgba;
};