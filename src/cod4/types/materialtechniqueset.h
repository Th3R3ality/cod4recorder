#pragma once
#include "../enums/materialworldvertexformat.h"
#include "materialtechnique.h"
struct MaterialTechniqueSet
{
	char* name;
	MaterialWorldVertexFormat worldVertFormat;
	bool hasBeenUploaded;
	char unused[1];
	MaterialTechniqueSet* remappedTechniqueSet;
	MaterialTechnique* techniques[34];
};