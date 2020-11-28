#include "ComponentCamera.h"
#include "Application.h"
	
ComponentCamera::ComponentCamera(Application* papaito,GameObject* parent) : Component(papaito,parent, CompCamera) 
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
			App->gobject->componentsToDelete.push_back(this);
			App->renderer3D->current_cam = nullptr;
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

void ComponentCamera::UpdateFrustum()
{
	if (gameObject != nullptr && gameObject->transform != nullptr)
	{
		frustum.pos = gameObject->transform->GetPos();
		frustum.front = gameObject->transform->GetRotation() * float3::unitZ;
		frustum.up = gameObject->transform->GetRotation() * float3::unitY;
	}
	else
	{
		frustum.pos = float3::zero;
		frustum.front = float3::unitZ;
		frustum.up = float3::unitY;
	}

}
