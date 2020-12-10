#pragma once
#include "Module.h"
#include <string>
#include "ComponentTexture.h"
#include "Resources.h"
#include <list>

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init();
	
	void SaveFile(uint size, char* output_file, ResourceType type, uint uuid, const char* path = nullptr);

	char* LoadFile(const char* path, ResourceType type, uint uuid);

	

public:

	std::list<Resource*> resources;
};