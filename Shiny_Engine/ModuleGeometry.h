#pragma once
#include "Module.h"
#include "Primitive.h"
#include "Globals.h"

class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Start();
	update_status Update(float dt);

	bool ret = true;
	void Draw();

	uint my_id = 0;
	uint my_indices = 0;
};