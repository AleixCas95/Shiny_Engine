#include "ModuleHierarchy.h"
#include "Application.h"



ModuleHierarchy::ModuleHierarchy(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleHierarchy::~ModuleHierarchy()
{
}

bool ModuleHierarchy::Start()
{
	return true;
}
update_status ModuleHierarchy::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleHierarchy::CleanUp()
{
	return true;
}

void ModuleHierarchy::Draw()
{
	if (ImGui::Begin("Hierarchy", &App->gui->showHierarchy, ImGuiWindowFlags_HorizontalScrollbar))
		ImGui::SetWindowPos(ImVec2(App->config->width * 0.75, App->config->height * 0 + 20));
	ImGui::SetWindowSize(ImVec2(App->config->width * 0.25, App->config->height * 0.30 - 20));
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("New Game Object"))
			{
				GameObject* newGO = new GameObject(App, App->gobject->root);
			}
			ImGui::MenuItem("Cancel");
			ImGui::EndMenu();
		}

		if (App->gobject->root)
		{
			for (auto child : App->gobject->root->childs)
				DrawGameObjects(child);
		}
	}
	ImGui::End();
}

void ModuleHierarchy::DrawGameObjects(GameObject* current)
{
	uint flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	if (current->childs.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (current == App->scene->current_object)
		flags |= ImGuiTreeNodeFlags_Selected;

	char name[256];

	sprintf_s(name, 256, "%s##%u", current->name.c_str(), current->uuid);

	if (ImGui::TreeNodeEx(name, flags))
	{
		if (ImGui::IsItemClicked(0))
		{
			App->scene->current_object = current;
		}

		for (std::vector<GameObject*>::iterator childs = current->childs.begin(); childs != current->childs.end(); ++childs)
		{
			DrawGameObjects(*childs);
		}
		ImGui::TreePop();
	}
}
