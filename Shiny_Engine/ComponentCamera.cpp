#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleTime.h"


ComponentCamera::ComponentCamera(Application* app_parent, GameObject* parent) : Component(app_parent, parent, CompCamera)
{
	parent->components.push_back(this);

	frustum.type = PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.f * atan(tan(frustum.verticalFov * 0.5f) * (float(App->window->width) / App->window->height));
	LOG("%i", App->window->height);

}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Inspector()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (frustum.type != InvalidFrustum)
		{
			char* camType = frustum.type == PerspectiveFrustum ? "Perspective" : "Orthographic";
			ImGui::Text("Camera type: ", camType);
			ImGui::DragFloat("Far plane distance", &frustum.farPlaneDistance, 10, frustum.nearPlaneDistance, 2000.0f);
			ImGui::DragFloat("Near plane distance", &frustum.nearPlaneDistance, 1, 0.1, frustum.farPlaneDistance);

			float fov = frustum.verticalFov * RADTODEG;
			if (ImGui::DragFloat("FOV", &fov, 1, 55, 120))
			{
				frustum.verticalFov = fov * DEGTORAD;
				frustum.horizontalFov = 2.f * atan(tan(frustum.verticalFov * 0.5f) * (float(App->window->width) / App->window->height));
			}
		}
		else
		{
			ImGui::Text("Invalid camera type");
		}

		if (ImGui::Button("Delete Camera"))
		{
			if (App->module_time->gameState == GameState::EDITOR)
				App->gobject->componentsToDelete.push_back(this);

		}
	}
}

float4x4 ComponentCamera::GetViewMatrix()
{
	float4x4 matrix;

	matrix = frustum.ViewMatrix();
	matrix.Transpose();
	return matrix;
}

float4x4 ComponentCamera::GetProjectionMatrix()
{
	float4x4 matrix;

	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();
	return matrix;
}

void ComponentCamera::Save(JSON_Object* parent)
{
	json_object_set_number(parent, "Type", type);
	json_object_set_number(parent, "UUID", uuid);

	JSON_Value* pos = json_value_init_object();
	JSON_Object* positionObj = json_value_get_object(pos);

	json_object_set_value(parent, "Position", pos);

	json_object_set_number(positionObj, "X", frustum.pos.x);
	json_object_set_number(positionObj, "Y", frustum.pos.y);
	json_object_set_number(positionObj, "Z", frustum.pos.z);

	json_object_set_number(parent, "nearPlaneDistance", frustum.nearPlaneDistance);
	json_object_set_number(parent, "farPlaneDistance", frustum.farPlaneDistance);
	json_object_set_number(parent, "verticalFov", frustum.verticalFov);
	json_object_set_number(parent, "horizontalFov", frustum.horizontalFov);
}

void ComponentCamera::Load(JSON_Object* parent)
{
	uuid = json_object_get_number(parent, "UUID");

	JSON_Object* pos = json_object_get_object(parent, "Position");
	frustum.pos.x = json_object_get_number(pos, "X");
	frustum.pos.y = json_object_get_number(pos, "Y");
	frustum.pos.z = json_object_get_number(pos, "Z");

	frustum.nearPlaneDistance = json_object_get_number(parent, "nearPlaneDistance");
	frustum.farPlaneDistance = json_object_get_number(parent, "farPlaneDistance");
	frustum.verticalFov = json_object_get_number(parent, "verticalFov");
	frustum.horizontalFov = json_object_get_number(parent, "horizontalFov");
}

void ComponentCamera::UpdateFrustum()
{
	if (gameObject != nullptr && gameObject->transform != nullptr)
	{
		frustum.pos = gameObject->transform->GetPos();
		frustum.front = gameObject->transform->GetRotationQuat() * float3::unitZ;
		frustum.up = gameObject->transform->GetRotationQuat() * float3::unitY;
	}
	else
	{
		frustum.pos = float3::zero;
		frustum.front = float3::unitZ;
		frustum.up = float3::unitY;
	}

}

void ComponentCamera::SetFOV(float fov) {

	frustum.verticalFov = DEGTORAD * fov;
	frustum.horizontalFov = 2.0f * atanf(0.0f * tanf(frustum.verticalFov * 0.5f));
}

float ComponentCamera::GetFOV() const {

	return RADTODEG * frustum.verticalFov;
}



void ComponentCamera::SetNearPlane(float near_plane) {

	if (near_plane >= frustum.farPlaneDistance)
		near_plane = frustum.farPlaneDistance - 1.0f;

	if (near_plane < 0.0f)
		near_plane = 0.1f;

	frustum.nearPlaneDistance = near_plane;
}


float ComponentCamera::GetNearPlane() const {

	return frustum.nearPlaneDistance;
}


void ComponentCamera::SetFarPlane(float far_plane) {

	if (far_plane <= frustum.nearPlaneDistance)
		far_plane = frustum.nearPlaneDistance + 1.0f;

	frustum.farPlaneDistance = far_plane;
}


float ComponentCamera::GetFarPlane() const {

	return frustum.farPlaneDistance;
}