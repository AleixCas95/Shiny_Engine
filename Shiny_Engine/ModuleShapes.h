#pragma once
#include "ModuleGUI.h"
#include "ImGui/imgui.h"

class ModuleShapes : public Module
{
public:
	ModuleShapes(Application* app, bool start_enabled = false);
	~ModuleShapes();

	void Draw();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	scriptType uuid;
};

