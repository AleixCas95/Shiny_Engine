#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(Application* app_parent, GameObject* parent);
	~ComponentCamera();

	void Inspector();

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

	void Save(JSON_Object* parent);

	void Load(JSON_Object* parent);

	void UpdateFrustum();

	void SetFOV(float fov);
	float GetFOV() const;
	void SetNearPlane(float near_plane);
	float GetNearPlane() const;
	void SetFarPlane(float far_plane);
	float GetFarPlane() const;

public:

	Frustum frustum;
};