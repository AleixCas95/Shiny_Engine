#include "ComponentTexture.h"
#include "Application.h"


ComponentTexture::ComponentTexture(Application* app_parent, GameObject* parent) : Component(app_parent, parent, CompTexture)
{
	parent->components.push_back(this);
}


ComponentTexture::~ComponentTexture()
{
}

void ComponentTexture::Inspector()
{
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Texture Active", &print);
		ImGui::Text("%s", path.c_str());
		ImGui::Image((void*)(intptr_t)RTexture->id, ImVec2(225, 225), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::Checkbox("Checkers", &checkers);
		ImGui::Text("Resource used %i times", RTexture->usage);

		if (ImGui::Button("Delete Texture"))
		{
			App->gobject->componentsToDelete.push_back(this);
		}
	}
}

unsigned int ComponentTexture::GetID()
{
	return checkers == false ? RTexture->id : App->fbx->checkerImageID;
}
void ComponentTexture::Save(JSON_Object* parent)
{
	json_object_set_number(parent, "Type", type);
	json_object_set_number(parent, "UUID", uuid);
	
	json_object_set_string(parent, "Path", path.c_str());
}

void ComponentTexture::Load(JSON_Object* parent)
{
	uuid = json_object_get_number(parent, "UUID");

	path = json_object_get_string(parent, "Path");
	
	RTexture = new ResourceTexture(path.c_str());

	App->fbx->RealLoadTexture(path.c_str(), RTexture->id);

	App->resources->AddResource(RTexture);
	
}
