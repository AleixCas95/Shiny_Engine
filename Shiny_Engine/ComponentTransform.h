#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ImGui/imgui.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(Application* papaito, GameObject* parent);
	~ComponentTransform();

	void Inspector();

	void SetPos(float x, float y, float z);
	void SetPos(float3 position);
	void Move(float3 distance);

	float3 GetPos() const;
	float3 GetGlobalPos() const;

	void SetScale(float x, float y, float z);
	void SetScale(float3 scale);

	void Scale(float3 scale);

	float3 GetScale();
	float3 GetGlobalScale();

	void SetRotation(Quat rotation);
	void SetRotation(float3 rotation);

	void Rotate(Quat rotation);

	Quat GetRotation() const;
	Quat GetGlobalRotation() const;

	void SetTransform(float4x4 trans);
	void SetIdentity();

	float4x4 GetMatrixOGL() const;
	float4x4 GetMatrix() const;

	float4x4 GetLocalMatrix() const;

public:
	GameObject* parent = nullptr;
	AABB bounding_box;
	AABB original_box;

private:
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;
};


