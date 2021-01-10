#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"
#include "imgui/imgui.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(Application* app_parent, GameObject* parent);
	~ComponentTransform();

	void Inspector();

	void SetPos(float x, float y, float z);
	void SetPos(float3 position);
	void Move(float3 distance);

	void SumRotation(float3 rot);

	float3 GetPos() const;
	float3 GetGlobalPos() const;
	float3 GetGlobalRot() const;

	void SetScale(float x, float y, float z);
	void SetScale(float3 scale);

	void Scale(float3 scale);

	float3 GetScale();
	float3 GetGlobalScale();

	void SetRotation(Quat rotation);
	void SetRotation(float3 rotation);

	void Rotate(Quat rotation);

	float3 GetRotation() const;

	Quat GetRotationQuat() const;
	Quat GetGlobalRotation() const;

	void SumPositionGlobal(float3 pos);
	void SumPositionLocal(float3& dir, float vel);

	void SetTransform(float4x4 trans);
	void SetIdentity();

	float4x4 GetMatrixOGL() const;
	float4x4 GetMatrix() const;

	float4x4 GetLocalMatrix() const;

	void UpdateBoundingBox();

	void Save(JSON_Object* parent);

	void Load(JSON_Object* parent);

	void GuizmoOptions();


	float3 position = float3::zero;
	Quat rotation_quat = Quat::identity;
	float3 scale = float3::one;
	float3 rotation = float3::zero;
};



