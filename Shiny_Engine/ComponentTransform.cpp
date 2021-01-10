#include "ComponentTransform.h"
#include "GameObject.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Application.h"

ComponentTransform::ComponentTransform(Application* app_parent, GameObject* parent) : Component(app_parent, parent, CompTransform)
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
			UpdateBoundingBox();
		}
		if (ImGui::DragFloat3("Scale", &scale[0], 0.1f, 0.0f, 0.0f, "%.2f"))
		{
			UpdateBoundingBox();
		}
		float3 degRotation = rotation_quat.ToEulerXYZ();
		degRotation = RadToDeg(degRotation);
		if (ImGui::DragFloat3("Rotation", &degRotation[0], 0.1f, 0.0f, 0.0f, "%.2f"))
		{
			SetRotation(DegToRad(degRotation));
			UpdateBoundingBox();
		}
		if (ImGui::Button("Reset"))
		{
			position = float3::zero;
			rotation_quat = Quat::identity;
			scale = float3::one;
			UpdateBoundingBox();
		}

		ImGui::Separator();

		ImGui::Checkbox("Static", &gameObject->isStatic);

		ImGui::Separator();

		GuizmoOptions();
	}
}

void ComponentTransform::SetPos(float x, float y, float z)
{
	SetPos(float3(x, y, z));
}

void ComponentTransform::SetPos(float3 position)
{
	this->position = position;
	UpdateBoundingBox();
}

void ComponentTransform::Move(float3 distance)
{
	this->position = this->position.Add(distance);
	UpdateBoundingBox();
}

void ComponentTransform::SumRotation(float3 rot)
{
	rotation_quat = rotation_quat * Quat::FromEulerXYZ(rot.x * DEGTORAD, rot.y * DEGTORAD, rot.z * DEGTORAD);
	rotation = rotation_quat.ToEulerXYZ() * RADTODEG;
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

float3 ComponentTransform::GetGlobalRot() const
{
	float3 ret = float3::zero;
	Quat glob_q = rotation_quat;

	GameObject* aux = gameObject->GetParent();

	while (aux != nullptr) {

		if (aux->transform != nullptr) {
			float3 p_rot = aux->transform->GetRotation();
			glob_q = glob_q * Quat::FromEulerXYZ(p_rot.x * DEGTORAD, p_rot.y * DEGTORAD, p_rot.z * DEGTORAD);
			ret = glob_q.ToEulerXYZ() * RADTODEG;
		}

		aux = aux->GetParent();
	}

	return ret;
}

void ComponentTransform::SumPositionGlobal(float3 pos)
{
	position += pos;
}

void ComponentTransform::SumPositionLocal(float3& dir, float vel)
{
	if (rotation.x == -180.0f)
		rotation.x = 0.0f;
	if (rotation.y == -180.0f)
		rotation.y = 0.0f;
	if (rotation.z == -180.0f)
		rotation.z = 0.0f;

	float3 rot = rotation * DEGTORAD;
	float4x4 vec; //Matrix to store the values needed when calculation the rotation

	float3 aux_dir = { 0.0f,0.0f,0.0f };

	if (rot.z > 0.01f || rot.z < -0.01f) {
		vec.SetRotatePartZ(rot.z);
		aux_dir.x += dir.x * vec[0][0] + dir.y * vec[0][1];
		aux_dir.y += dir.x * vec[1][0] + dir.y * vec[1][1];
	}

	if (rot.y > 0.01f || rot.y < -0.01f) {
		vec.SetRotatePartY(rot.y);
		aux_dir.x += dir.x * vec[0][0] + dir.z * vec[0][2];
		aux_dir.z += dir.x * vec[2][0] + dir.z * vec[2][2];
	}

	if (rot.x > 0.01f || rot.x < -0.01f) {
		vec.SetRotatePartX(rot.x);
		aux_dir.y += dir.y * vec[1][1] + dir.z * vec[1][2];
		aux_dir.z += dir.y * vec[2][1] + dir.z * vec[2][2];
	}
	if (aux_dir.IsZero())
		aux_dir = dir;

	aux_dir.Normalize();
	dir = aux_dir;

	float3 pos = dir * vel;

	position += pos;
}

void ComponentTransform::SetScale(float x, float y, float z)
{
	scale = float3(x, y, z);
	UpdateBoundingBox();
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	UpdateBoundingBox();
}

void ComponentTransform::Scale(float3 scale)
{
	this->scale = this->scale.Mul(scale);
	UpdateBoundingBox();
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
	this->rotation_quat = rotation;
	UpdateBoundingBox();
}

void ComponentTransform::SetRotation(float3 rotation)
{
	this->rotation_quat = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	UpdateBoundingBox();
}

void ComponentTransform::Rotate(Quat rotation)
{
	this->rotation_quat = rotation.Mul(this->rotation_quat).Normalized();
	UpdateBoundingBox();
}

Quat ComponentTransform::GetRotationQuat() const
{
	return rotation_quat;
}

float3 ComponentTransform::GetRotation() const {

	return rotation;
}

Quat ComponentTransform::GetGlobalRotation() const
{
	if (gameObject->parent)
	{
		return rotation_quat.Mul(gameObject->parent->transform->GetGlobalRotation());
	}
	return rotation_quat;
}

void ComponentTransform::SetTransform(float4x4 trans)
{
	trans.Decompose(position, rotation_quat, scale);
	UpdateBoundingBox();
}

void ComponentTransform::SetIdentity()
{
	position = float3::zero;
	rotation_quat = Quat::identity;
	scale = float3::one;
	UpdateBoundingBox();
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
	return float4x4::FromTRS(position, rotation_quat, scale);
}

void ComponentTransform::UpdateBoundingBox()
{
	OBB obb;
	obb = gameObject->originalBoundingBox.ToOBB();
	obb.Transform(GetMatrix());

	gameObject->boundingBox = obb.MinimalEnclosingAABB();

	for (std::list<GameObject*>::iterator it = gameObject->childs.begin(); it != gameObject->childs.end(); ++it)
	{
		(*it)->transform->UpdateBoundingBox();
	}
}

void ComponentTransform::Save(JSON_Object* parent)
{
	json_object_set_number(parent, "Type", type);
	json_object_set_number(parent, "UUID", uuid);

	// Position
	//------------------------------------------------------------------------
	JSON_Value* pos = json_value_init_object();
	JSON_Object* positionObj = json_value_get_object(pos);

	json_object_set_value(parent, "Position", pos);

	json_object_set_number(positionObj, "X", position.x);
	json_object_set_number(positionObj, "Y", position.y);
	json_object_set_number(positionObj, "Z", position.z);
	//------------------------------------------------------------------------

	// Rotation
	//------------------------------------------------------------------------
	JSON_Value* rot = json_value_init_object();
	JSON_Object* rotationObj = json_value_get_object(rot);

	json_object_set_value(parent, "Rotation", rot);

	json_object_set_number(rotationObj, "X", rotation_quat.x);
	json_object_set_number(rotationObj, "Y", rotation_quat.y);
	json_object_set_number(rotationObj, "Z", rotation_quat.z);
	json_object_set_number(rotationObj, "W", rotation_quat.w);
	//------------------------------------------------------------------------

	// Scale
	//------------------------------------------------------------------------
	JSON_Value* scal = json_value_init_object();
	JSON_Object* scaleObj = json_value_get_object(scal);

	json_object_set_value(parent, "Scale", scal);

	json_object_set_number(scaleObj, "X", scale.x);
	json_object_set_number(scaleObj, "Y", scale.y);
	json_object_set_number(scaleObj, "Z", scale.z);
	//------------------------------------------------------------------------
}

void ComponentTransform::Load(JSON_Object* parent)
{
	uuid = json_object_get_number(parent, "UUID");

	// Position
	//------------------------------------------------------------------------
	JSON_Object* pos = json_object_get_object(parent, "Position");
	position.x = json_object_get_number(pos, "X");
	position.y = json_object_get_number(pos, "Y");
	position.z = json_object_get_number(pos, "Z");
	//------------------------------------------------------------------------

	// Scale
	//------------------------------------------------------------------------
	JSON_Object* scal = json_object_get_object(parent, "Scale");
	scale.x = json_object_get_number(scal, "X");
	scale.y = json_object_get_number(scal, "Y");
	scale.z = json_object_get_number(scal, "Z");
	//------------------------------------------------------------------------

	// Rotation
	//------------------------------------------------------------------------
	JSON_Object* rot = json_object_get_object(parent, "Rotation");
	rotation_quat.x = json_object_get_number(rot, "X");
	rotation_quat.y = json_object_get_number(rot, "Y");
	rotation_quat.z = json_object_get_number(rot, "Z");
	rotation_quat.w = json_object_get_number(rot, "W");
	//------------------------------------------------------------------------
}

void ComponentTransform::GuizmoOptions()
{
	if (ImGui::RadioButton("None", App->scene->guiz_operation == ImGuizmo::BOUNDS))
	{
		App->scene->guiz_operation = ImGuizmo::BOUNDS;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Move", App->scene->guiz_operation == ImGuizmo::TRANSLATE))
	{
		App->scene->guiz_operation = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", App->scene->guiz_operation == ImGuizmo::SCALE))
	{
		App->scene->guiz_operation = ImGuizmo::SCALE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", App->scene->guiz_operation == ImGuizmo::ROTATE))
	{
		App->scene->guiz_operation = ImGuizmo::ROTATE;
	}
}