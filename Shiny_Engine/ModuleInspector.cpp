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
	if (ImGui::Begin("Inspector", &App->gui->showInspector,ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (App->scene->current_object)
		{
			char buf[64];
			strcpy_s(buf, 64, App->scene->current_object->name.c_str());
			if (ImGui::InputText("##Name", buf, (int)(sizeof(buf) / sizeof(*buf))))
			{
				App->scene->current_object->name = buf;
			}
			App->scene->current_object->transform->Inspector();
			for (std::list<Component*>::iterator it = App->scene->current_object->components.begin(); it != App->scene->current_object->components.end(); ++it)
			{
				(*it)->Inspector();
			}
		}
	}
	ImGui::End();
}