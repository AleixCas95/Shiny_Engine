#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"


ComponentTransform::ComponentTransform(Application* papaito, GameObject* parent) : Component(papaito, parent, CompTransform)
{

}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Inspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Position", &position[0], 0.1f, 0.0f, 0.0f, "%.2f"))
		{

		}
		float3 degRotation = rotation.ToEulerXYZ();
		degRotation = RadToDeg(degRotation);
		if (ImGui::DragFloat3("Rotation", &degRotation[0], 0.1f, 0.0f, 0.0f, "%.2f"))
		{
			SetRotation(DegToRad(degRotation));
		}
		if (ImGui::DragFloat3("Scale", &scale[0], 0.1f, 0.0f, 0.0f, "%.2f"))
		{

		}

		if (ImGui::Button("Reset"))
		{
			position = float3::zero;
			rotation = Quat::identity;
			scale = float3::one;
		}
	}
}

void ComponentTransform::SetPos(float x, float y, float z)
{
	SetPos(float3(x, y, z));
}

void ComponentTransform::SetPos(float3 position)
{
	this->position = position;
}

void ComponentTransform::Move(float3 distance)
{
	this->position = this->position.Add(position);
}

float3 ComponentTransform::GetPos() const
{
	return position;
}

float3 ComponentTransform::GetGlobalPos() const
{
	float3 pos;
	GetMatrix().Decompose(pos, Quat(), float3());

	return pos;
}

void ComponentTransform::SetScale(float x, float y, float z)
{
	scale = float3(x, y, z);
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
}

void ComponentTransform::Scale(float3 scale)
{
	this->scale = this->scale.Mul(scale);
}

float3 ComponentTransform::GetScale()
{
	return scale;
}

float3 ComponentTransform::GetGlobalScale()
{
	if (gameObject->parent)
	{
		return scale.Mul(gameObject->parent->transform->GetGlobalScale());
	}
	return scale;
}

void ComponentTransform::SetRotation(Quat rotation)
{
	this->rotation = rotation;
}

void ComponentTransform::SetRotation(float3 rotation)
{
	this->rotation = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
}

void ComponentTransform::Rotate(Quat rotation)
{
	this->rotation = rotation.Mul(this->rotation).Normalized();
}

Quat ComponentTransform::GetRotation() const
{
	return rotation;
}

Quat ComponentTransform::GetGlobalRotation() const
{
	if (gameObject->parent)
	{
		return rotation.Mul(gameObject->parent->transform->GetGlobalRotation());
	}
	return rotation;
}

void ComponentTransform::SetTransform(float4x4 trans)
{
	trans.Decompose(position, rotation, scale);
}

void ComponentTransform::SetIdentity()
{
	position = float3::zero;
	rotation = Quat::identity;
	scale = float3::one;
}

float4x4 ComponentTransform::GetMatrixOGL() const
{
	return GetMatrix().Transposed();
}

float4x4 ComponentTransform::GetMatrix() const
{
	float4x4 localMatrix = GetLocalMatrix();
	if (gameObject->parent)
	{
		return gameObject->parent->transform->GetMatrix().Mul(localMatrix);
	}
	return localMatrix;
}

float4x4 ComponentTransform::GetLocalMatrix() const
{
	return float4x4::FromTRS(position, rotation, scale);
}


void ComponentTransform::Save(JSON_Object* parent)
{

}

void ComponentTransform::Load(JSON_Object* parent)
{

}

