#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentCamera :
	public Component 
{
public:
	ComponentCamera(Application* papaito , GameObject* parent);
	~ComponentCamera();

	void Inspector();

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

public:

	Frustum frustum;
};