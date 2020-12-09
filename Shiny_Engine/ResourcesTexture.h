#pragma once
#include "Resources.h"

class ResourceTexture :
	public Resource
{
public:
	ResourceTexture(const char* path);
	~ResourceTexture();

	void Unload();

public:
	uint id = 0u;
};