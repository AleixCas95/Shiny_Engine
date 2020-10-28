#include "ComponentTransform.h"
#include "GameObject.h"


ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent, CompTransform)
{

}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Inspector()
{
	
}

void ComponentTransform::SetPos(float x, float y, float z)
{
	
}

void ComponentTransform::SetPos(float3 position)
{

}

void ComponentTransform::Move(float3 distance)
{
	
}

float3 ComponentTransform::GetPos() const
{
	return position;
}

float3 ComponentTransform::GetGlobalPos() const
{
	

	return pos;
}

void ComponentTransform::SetScale(float x, float y, float z)
{
	
}

void ComponentTransform::SetScale(float3 scale)
{

}

void ComponentTransform::Scale(float3 scale)
{

}

float3 ComponentTransform::GetScale()
{
	return scale;
}

float3 ComponentTransform::GetGlobalScale()
{
	
	return scale;
}

void ComponentTransform::SetRotation(Quat rotation)
{
	
}

void ComponentTransform::SetRotation(float3 rotation)
{
	
}

void ComponentTransform::Rotate(Quat rotation)
{
	
}

Quat ComponentTransform::GetRotation() const
{
	return rotation;
}

Quat ComponentTransform::GetGlobalRotation() const
{

	return rotation;
}

void ComponentTransform::SetTransform(float4x4 trans)
{
	

void ComponentTransform::SetIdentity()
{
	
}

float4x4 ComponentTransform::GetMatrixOGL() const
{
	
}

float4x4 ComponentTransform::GetMatrix() const
{
	
	return localMatrix;
}

float4x4 ComponentTransform::GetLocalMatrix() const
{
	return float4x4::FromTRS(position, rotation, scale);
}
