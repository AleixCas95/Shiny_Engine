#pragma once
#include "ImGui/imgui.h"
#include "ModuleGUI.h"

class GameObject;

class ModuleHierarchy : public Module
{
public:
	ModuleHierarchy(Application* app, bool start_enabled = false);
	~ModuleHierarchy();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();

	void DrawGameObjects(GameObject* current);
};