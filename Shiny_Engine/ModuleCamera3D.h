#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ComponentCamera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LookAt(const float3& Spot);

public:

	ComponentCamera* compCamera = nullptr;
	GameObject* cameraGO = nullptr;

private:
	int cameraSpeed = 3;
};