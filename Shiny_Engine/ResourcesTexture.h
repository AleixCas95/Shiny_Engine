#ifndef __ResourceTexture_H__
#define __ResourceTexture_H__

#include "Resources.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture(scriptType uuid);
	~ResourceTexture();

	bool LoadInMemory();
	bool UnloadInMemory();

public:
	uint tex_id = 0;
	int tex_width, tex_height;
	uint id = 0u;
};

#endif