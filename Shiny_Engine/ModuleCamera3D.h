#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void Look(const math::float3& Position, const math::float3& Reference, bool RotateAroundReference = false);
	void LookAt(const math::float3& Spot);
	void Move(const math::float3& Movement);
	void Orbit(math::float3 target, float deltaX, float deltaY);
	float* GetViewMatrix();


private:

	void CalculateViewMatrix();

public:

	math::float3 X, Y, Z, Position, Reference;

	float camera_mov_speed = 3.0f;
	float mouse_wheel_speed = 0.5f;
	float mouse_sensitivity = 0.25f;

private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
};