#pragma once
#include "ModuleGUI.h"
#include "imgui/imgui.h"

class ModuleInspector : public Module
{
public:
	ModuleInspector(Application* app, bool start_enabled = true);
	~ModuleInspector();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();
};

