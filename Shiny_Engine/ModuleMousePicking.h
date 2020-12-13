#pragma once
#include "Module.h"
class ModuleMousePicking : public Module
{
public:
	ModuleMousePicking(Application* app, bool start_enabled = true);
	~ModuleMousePicking();

	update_status Update(float dt);
};

