#include "Application.h"
#include "ComponentGraphScript.h"
#include "ResourceGraphManager.h"
#include "Component.h"
#include "imgui/imgui.h"


ComponentGraphScript::ComponentGraphScript(Application* app_parent, GameObject* parent,uint scriptNum) : Component(app_parent, parent, CompGraphScript)
{
	type = Object_Type::CompGraphScript;
	gobjects.push_back(gameObject);

	this->scriptNum = scriptNum;
	
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


void ComponentGraphScript::Save(JSON_Array* comp_array) const {

	JSON_Value* value = json_value_init_object();
	JSON_Object* obj = json_value_get_object(value);

	json_object_set_number(obj, "Component Type", type);
	json_object_set_number(obj, "Script UUID", uuid_script);
	json_object_set_string(obj, "Name", script_name);
	json_object_set_boolean(obj, "Active", active);

	//Save Blackboard
	JSON_Value* value_arr = json_value_init_array();
	JSON_Array* array = json_value_get_array(value_arr);

	for (uint i = 1; i < gobjects.size(); i++) {

		JSON_Value* aux_val = json_value_init_object();
		JSON_Object* aux_obj = json_value_get_object(aux_val);

		GameObject* it = gobjects[i];

		json_object_set_number(aux_obj, "GO UUID", it->uuid);

		json_array_append_value(array, aux_val);
	}

	json_object_set_value(obj, "Blackboard", value_arr);

	json_array_append_value(comp_array, value);

	//Save .script file with all the info of nodes and links
	//SaveScriptFile(uuid_script);

}



void ComponentGraphScript::Load(JSON_Object* comp_obj) {

	uuid_script = json_object_get_number(comp_obj, "Script UUID");
	
	active = json_object_get_boolean(comp_obj, "Active");
	std::strcpy(script_name, json_object_get_string(comp_obj, "Name"));

	ResourceGraphManager* res = (ResourceGraphManager*)App->resources->Get(uuid_script);

	if (res)
		res->LoadToMemory();

	//Load Blackboard
	JSON_Array* array_bb = json_object_get_array(comp_obj, "Blackboard");

	JSON_Object* it;

	for (uint i = 0; i < json_array_get_count(array_bb); i++) {

		it = json_array_get_object(array_bb, i);
		scriptType uuid_aux = json_object_get_number(it, "GO UUID");

		if (uuid_aux != 0)
			uuidsLoad.push_back(uuid_aux);

	}
	
}

