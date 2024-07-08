#pragma once
#include "materialvertexdeclaration.h"
#include "materialvertexshader.h"
#include "materialpixelshader.h"
#include "materialshaderargument.h"

#pragma pack(push, 4)
struct MaterialPass
{
	MaterialVertexDeclaration* vertexDecl;
	MaterialVertexShader* vertexShader;
	MaterialPixelShader* pixelShader;
	char perPrimArgCount;
	char perObjArgCount;
	char stableArgCount;
	char customSamplerFlags;
	MaterialShaderArgument* args;
};
#pragma pack(pop)