#include "ComponentTexture.h"
#include "Application.h"


ComponentTexture::ComponentTexture(Application* papaito, GameObject* parent) : Component(papaito, parent, CompTexture)
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
		ImGui::Checkbox("Active", &print);
		ImGui::Text("%s", path.c_str());
		ImGui::Image((void*)(intptr_t)tex_id, ImVec2(225, 225), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::Checkbox("Checkers", &checkers);
	}
}

void ComponentTexture::Save(JSON_Object* parent)
{
	json_object_set_number(parent, "Type", type);
	json_object_set_number(parent, "UUID", uuid);
}

void ComponentTexture::Load(JSON_Object* parent)
{
	uuid = json_object_get_number(parent, "UUID");

	path = json_object_get_string(parent, "Path");
}
