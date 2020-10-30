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
	{
	}
	ImGui::End();
}

void ModuleHierarchy::DrawGameObjects(GameObject* current)
{

}
