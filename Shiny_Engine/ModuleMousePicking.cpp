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

				for (std::list<GameObject*>::iterator it = App->gobject->gameObjects.begin(); it != App->gobject->gameObjects.end(); ++it)
				{
					if ((*it)->active && (*it)->HasComponent(Object_Type::CompMesh))
					{
						LineSegment ray(picking);

						if (ray.Intersects((*it)->boundingBox))
						{
							candidates.push_back(*it);
						}
					}
				}

				for (std::list<GameObject*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
				{
					LineSegment ray(picking);
					ray.Transform((*it)->transform->GetMatrix().Inverted());

					ComponentMesh* mesh = (ComponentMesh*)(*it)->GetComponent(Object_Type::CompMesh);

					if (mesh)
					{
						Triangle triangle;
						for (int i = 0; i < mesh->mesh->index.size / 3; ++i)
						{
							triangle.a.x = mesh->mesh->vertex.data[mesh->mesh->index.data[i * 3] * 3];
							triangle.a.y = mesh->mesh->vertex.data[mesh->mesh->index.data[i * 3] * 3 + 1];
							triangle.a.z = mesh->mesh->vertex.data[mesh->mesh->index.data[i * 3] * 3 + 2];

							float distance;
							float3 position;

							if (ray.Intersects(triangle, &distance, &position))
							{
								if (distance < smallerDist || smallerDist == -1.0f)
								{
									smallerDist = distance;
									closestObject = (*it);
								}
							}
						}
					}
				}
				App->scene->current_object = closestObject;

			}
		}
	}

	return UPDATE_CONTINUE;
}