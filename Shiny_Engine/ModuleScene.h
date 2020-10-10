#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"


#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	uint my_id = 0;
	uint my_indices = 0;
	uint* indices = nullptr;
	float* vertex = nullptr;

};
#pragma once
