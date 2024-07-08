#pragma once
#include <d3d9.h>
#include "gfximageloaddef.h"
union GfxTexture
{
	IDirect3DBaseTexture9* basemap;
	IDirect3DTexture9* map;
	IDirect3DVolumeTexture9* volmap;
	IDirect3DCubeTexture9* cubemap;
	GfxImageLoadDef* loadDef;
	void* data;
};