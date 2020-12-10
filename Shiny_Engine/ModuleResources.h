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

public:

	std::list<Resource*> resources;
};