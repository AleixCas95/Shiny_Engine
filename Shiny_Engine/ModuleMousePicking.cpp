#include "ModuleMousePicking.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleGameObject.h"
#include "ComponentMesh.h"


ModuleMousePicking::ModuleMousePicking(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleMousePicking::~ModuleMousePicking()
{
}

update_status ModuleMousePicking::Update()
{

	if (!ImGui::IsWindowHovered())
	{
		if (App->renderer3D->current_cam == App->camera->compCamera)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
			{
				float mouseX = -(1.f - ((float(App->input->GetMouseX()) * 2.f) / (float)App->window->width));
				float mouseY = (1.f - ((float(App->input->GetMouseY()) * 2.f) / (float)App->window->height));

				LineSegment picking = App->camera->compCamera->frustum.UnProjectLineSegment(mouseX, mouseY);

				float smallerDist = -1.0f;

				GameObject* closestObject = nullptr;

				std::list<GameObject*> candidates;

			}
		}
	}

	return UPDATE_CONTINUE;
}