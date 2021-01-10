#include "ComponentTexture.h"
#include "Application.h"
#include "ModuleFBX.h"
#include "ResourcesTexture.h"

ComponentTexture::ComponentTexture(Application* app_parent, GameObject* parent) : Component(app_parent, parent, CompTexture)
{
	parent->components.push_back(this);
}


ComponentTexture::~ComponentTexture()
{
	App->resources->ResourceUsageDecreased(RTexture);
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

	// Path
	//------------------------------------------------------------------------
	json_object_set_string(parent, "Path", path.c_str());
	//------------------------------------------------------------------------
}

void ComponentTexture::Load(JSON_Object* parent)
{
	
	uuid = json_object_get_number(parent, "UUID Material");
	active = json_object_get_boolean(parent, "Active");

	if (uuid != 0) {
		ResourceTexture* r_tex = (ResourceTexture*)App->resources->Get(uuid);
		r_tex->LoadToMemory();
	}
}