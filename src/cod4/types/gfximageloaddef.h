#pragma once
struct GfxImageLoadDef
{
	char levelCount;
	char flags;
	__int16 dimensions[3];
	int format;
	int resourceSize;
	char data[1];
};
