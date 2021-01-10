#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "GameObject.h"
#include "ImGuizmo/ImGuizmo.h"


#define MAX_SNAKE 2



class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(GameObject* parent);
	GameObject* GetRootGameObject() const;


public:
	uint my_id = 0;
	uint my_indices = 0;
	uint* indices = nullptr;
	float* vertex = nullptr;

	GameObject* GO_to_delete = nullptr;
	bool want_to_delete_go = false;
	bool want_to_load = false;



	GameObject* current_object = nullptr;
	GameObject* root_object = nullptr;

	ImGuizmo::OPERATION guiz_operation = ImGuizmo::BOUNDS;

	ImGuizmo::MODE guiz_mode = ImGuizmo::WORLD;
};
#pragma once
