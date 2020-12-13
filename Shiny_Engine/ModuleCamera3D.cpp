#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	cameraGO = new GameObject(App, false, "Editor Camera");

	compCamera = new ComponentCamera(App, cameraGO);

	compCamera->frustum.pos = float3(40.f, 40.f, 40.f);

	LookAt(float3::zero);

	App->renderer3D->current_cam = compCamera;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!


		float3 newPos(0, 0, 0);
		float speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 6.0f * dt;

		float3 forward = compCamera->frustum.front * speed;
		float3 right = compCamera->frustum.WorldRight() * speed;

		if (App->renderer3D->current_cam == App->camera->compCamera)
		{


			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += forward;
				if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= forward;

				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= right;
				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += right;

				if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos += float3::unitY * speed;
				if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos -= float3::unitY * speed;

				compCamera->frustum.Translate(newPos);
			}

			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
			{
				if (App->scene->current_object)
				{
					float3 posCam = App->scene->current_object->transform->GetGlobalPos();

					compCamera->frustum.pos.x = posCam.x + 10;
					compCamera->frustum.pos.y = posCam.y + 10;
					compCamera->frustum.pos.z = posCam.z + 10;
					LookAt(posCam);
				}
			}

			if (App->input->GetMouseZ() != 0)
			{
				compCamera->frustum.pos += compCamera->frustum.front * cameraSpeed * App->input->GetMouseZ();
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT &&
				App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			{
				float Sensitivity = 0.1f;
				int dx = -App->input->GetMouseXMotion() * Sensitivity;
				int dy = -App->input->GetMouseYMotion() * Sensitivity;

				if (App->scene->current_object) {
					float3 object = App->scene->current_object->transform->GetGlobalPos();

					math::Quat rotationX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, -App->input->GetMouseXMotion() * Sensitivity * DEGTORAD);
					math::Quat rotationY = math::Quat::RotateAxisAngle(compCamera->frustum.WorldRight(), -App->input->GetMouseYMotion() * Sensitivity * DEGTORAD);
					math::Quat finalRotation = rotationX * rotationY;

					compCamera->frustum.up = finalRotation * compCamera->frustum.up;
					compCamera->frustum.front = finalRotation * compCamera->frustum.front;

					float distance = (compCamera->frustum.pos - object).Length();
					compCamera->frustum.pos = object + (-compCamera->frustum.front * distance);
				}
			}

			// Mouse motion ----------------

			else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				float Sensitivity = 0.001f;
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				if (dx != 0)
				{
					float DeltaX = (float)dx * Sensitivity;

					Quat rotation = Quat::RotateY(DeltaX);
					compCamera->frustum.front = rotation.Mul(compCamera->frustum.front).Normalized();
					compCamera->frustum.up = rotation.Mul(compCamera->frustum.up).Normalized();
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * Sensitivity;

					Quat rotation = Quat::RotateAxisAngle(compCamera->frustum.WorldRight(), DeltaY);

					float3 futureFrustumUp = rotation.Mul(compCamera->frustum.up).Normalized();

					if (futureFrustumUp.y > 0.0f)
					{
						compCamera->frustum.front = rotation.Mul(compCamera->frustum.front).Normalized();
						compCamera->frustum.up = futureFrustumUp;
					}
				}
			}
		}
	

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& Spot)
{
	float3 direction = Spot - compCamera->frustum.pos;

	float3x3 matrix = float3x3::LookAt(compCamera->frustum.front, direction.Normalized(), compCamera->frustum.up, float3::unitY);
	compCamera->frustum.front = matrix.MulDir(compCamera->frustum.front).Normalized();
	compCamera->frustum.up = matrix.MulDir(compCamera->frustum.up).Normalized();
}