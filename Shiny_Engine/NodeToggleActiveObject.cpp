#include "Application.h"
#include "NodeToggleActiveObject.h"
#include "GameObject.h"
#include "ModuleGui.h"


NodeToggleActiveObject::NodeToggleActiveObject(int id, const ImVec2& pos) : Node(id, "Action: ToggleActiveObject", pos, 1, 1, Node_Type_Action, Func_ToggleActiveObject)
{
}


NodeToggleActiveObject::~NodeToggleActiveObject()
{
}

bool NodeToggleActiveObject::Update(float dt, std::vector<GameObject*> BB_objects, uint num_comp_graph)
{
	node_state = Node_State_Idle;
	GameObject* object = nullptr;

	if (dt == 0.0f) {
		node_state = Node_State_ToUpdate;
		return true; //Show that node is updating, but nothing happens, because the simulation is paused
	}

	//If reference gets deleted, send error
	if (obj_indx >= BB_objects.size())
		node_state = Node_State_Error;
	else
		object = BB_objects[obj_indx];


	if (object) {
		switch (active_set) {
		case Toggle:
			object->SetActive(!object->IsActive());
			break;

		case True:
			object->SetActive(true);
			break;

		case False:
			object->SetActive(false);
			break;
		}

		node_state = Node_State_Updating;
	}
	else
		//	App->gui->AddLogToConsole("ERROR: Can't toggle active: Object is NULL");

		return true;
}

void NodeToggleActiveObject::Draw(std::vector<GameObject*> BB_objects)
{
	//GameObject reference
	DrawObjectsInstance(BB_objects);

	//Set active options
	static const char* active_set_str[] = { "Toggle", "True", "False" };

	if (ImGui::BeginCombo("Set active", curr_active_set_str)) {

		if (ImGui::Selectable("Toggle")) {
			curr_active_set_str = active_set_str[0];
			active_set = Toggle;
		}
		if (ImGui::Selectable("True")) {
			curr_active_set_str = active_set_str[1];
			active_set = True;
		}
		if (ImGui::Selectable("False")) {
			curr_active_set_str = active_set_str[2];
			active_set = False;
		}

		ImGui::EndCombo();
	}
}


void NodeToggleActiveObject::Save(JSON_Object* obj) const {

	json_object_set_boolean(obj, "using this", obj_using_this);
	json_object_set_number(obj, "id using", obj_indx);
	json_object_set_string(obj, "curr active set str", curr_active_set_str);
	json_object_set_number(obj, "active set", active_set);
}

void NodeToggleActiveObject::Load(JSON_Object* obj) {

	obj_using_this = json_object_get_boolean(obj, "using this");
	obj_indx = json_object_get_number(obj, "id using");
	curr_active_set_str = json_object_get_string(obj, "curr active set str");
	active_set = (ActiveSetTo)((int)json_object_get_number(obj, "active set"));
}