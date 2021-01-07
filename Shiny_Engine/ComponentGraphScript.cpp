#include "Application.h"
#include "ComponentGraphScript.h"
#include "imgui/imgui.h"


ComponentGraphScript::ComponentGraphScript(Application* app_parent, GameObject* parent) : Component(app_parent, parent, CompGraphScript)
{
	type = CompGraphScript;
}


ComponentGraphScript::~ComponentGraphScript()
{
}

void ComponentGraphScript::DrawInspector()
{
	//TODO: Close individual graph scripts
	if (ImGui::CollapsingHeader("Graph Script", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (has_script) {
			ImGui::Button("Drag script here");
			ImGui::Button("New Script", { 80,30 });
		}
	}

}
