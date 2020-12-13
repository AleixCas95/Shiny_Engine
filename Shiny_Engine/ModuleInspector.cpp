#include "ModuleInspector.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ImGui/imgui.h"

ModuleInspector::ModuleInspector(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
ModuleInspector::~ModuleInspector()
{}

bool ModuleInspector::Start()
{
	return true;
}
update_status ModuleInspector::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleInspector::CleanUp()
{
	return true;
}

void ModuleInspector::Draw()
{
	if (ImGui::Begin("Inspector", &App->gui->showInspector, ImGuiWindowFlags_HorizontalScrollbar))
		ImGui::SetWindowPos(ImVec2(App->config->width * 0.75, App->config->height * 0.30));
	ImGui::SetWindowSize(ImVec2(App->config->width * 0.25, App->config->height * 0.70));
	{
		if (App->scene->current_object)
		{
			char buf[64];
			strcpy_s(buf, 64, App->scene->current_object->name.c_str());
			if (ImGui::InputText("##Name", buf, (int)(sizeof(buf) / sizeof(*buf))))
			{
				App->scene->current_object->name = buf;
			}
			ImGui::Checkbox("Active", &App->scene->current_object->active);

			App->scene->current_object->transform->Inspector();

			for (std::list<Component*>::iterator it = App->scene->current_object->components.begin(); it != App->scene->current_object->components.end(); ++it)
			{
				(*it)->Inspector();
			}
			if (ImGui::BeginMenu("New Component"))
			{
				if (ImGui::MenuItem("Mesh"))
				{
					ComponentMesh* mesh = new ComponentMesh(App, App->scene->current_object);
				}
				if (ImGui::MenuItem("Texture"))
				{
					ComponentTexture* mesh = new ComponentTexture(App, App->scene->current_object);
				}
				if (ImGui::MenuItem("Camera"))
				{
					ComponentCamera* camera = new ComponentCamera(App, App->scene->current_object);
					App->renderer3D->play_cam = camera;
				}
				ImGui::MenuItem("Cancel");
				ImGui::EndMenu();
			}

			if (ImGui::Button("Delete Object"))
			{
				if (App->scene->current_object->parent)
				{
					App->scene->current_object->parent->childs.remove(App->scene->current_object);
				}
				NewObjectsToDelete(App->scene->current_object);
			}
		}
	}
	ImGui::End();
}

void ModuleInspector::NewObjectsToDelete(GameObject* object)
{
	App->gobject->gameObjectsToDelete.push_back(object);
	for (auto child : object->childs)
	{
		NewObjectsToDelete(child);
	}
}