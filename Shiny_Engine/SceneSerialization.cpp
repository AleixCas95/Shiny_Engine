#include "Application.h"
#include "SceneSerialization.h"
#include "ModuleScene.h"
#include "Component.h"
#include "ConfigEditor.h"
#include "ModuleResources.h"
#include "ComponentGraphScript.h"
#include "ModuleGui.h"
#include "ComponentCamera.h"

SceneSerialization::SceneSerialization() {

}


SceneSerialization::~SceneSerialization() {

}



void SceneSerialization::SaveScene(const char* scene_name) {

	LOG("Saving scene in %s.", scene_name)

		JSON_Value* root_value = json_value_init_array();

	//root_value = json_parse_file("scene1.json");

	//if (root_value == NULL) {
	//	LOG("JSON not found, creating new file");
	//	root_value = json_value_init_object();
	//}

	JSON_Array* array = json_value_get_array(root_value);

	SaveGOs(App->scene->GetRootGameObject(), array);



	json_serialize_to_file_pretty(root_value, scene_name);


	json_value_free(root_value);
}

void SceneSerialization::SaveSceneFromMesh(const char* scene_name, std::list<GameObject*> go_list) {

	LOG("Saving scene %s from mesh.", scene_name);



	JSON_Value* root_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(root_value);


	//Do individual GameObject->Save instead of iterating all
	for (std::list<GameObject*>::const_iterator it = go_list.begin(); it != go_list.end(); it++) {

		(*it)->SaveArr(array);
	}

	char folder_and_file[128];
	strcpy(folder_and_file, "Library/Scenes/");
	strcat(folder_and_file, scene_name);

	json_serialize_to_file_pretty(root_value, folder_and_file);

	json_value_free(root_value);
}

void SceneSerialization::LoadSceneFromMesh(const char* scene_name) //scene_name -> Complete path
{
	UID uuid = App->resources->GetUIDFromMeta(scene_name);
	std::string path_and_file = "Library/Scenes/" + std::to_string(uuid) + ".json";

	if (uuid != 0) {
		App->scene_serialization->LoadScene(path_and_file.c_str());
	}
	//#ifndef GAME_MODE
		//else
			//App->gui->AddLogToConsole("Couldn't charge scene. Drag something from the assets folder");
	//#endif
}

void SceneSerialization::LoadScene(const char* scene_name) {

	LOG("Loading scene %s.", scene_name);

	//Clean scene
	App->scene->DestroyGOs();
	//App->camera->cameraGO->components.clear();

	//Load scene
	JSON_Value* root_value = json_parse_file(scene_name);
	JSON_Array* array = json_value_get_array(root_value);
	JSON_Object* object;

	std::vector<GameObject*>go_list;
	int array_size = json_array_get_count(array);
	go_list.reserve(array_size);

	App->scene->CreateRootObject();

	//Create GOs without hierarchy
	for (uint i = 0; i < array_size; i++) {

		object = json_array_get_object(array, i);
		GameObject* aux = App->scene->CreateGameObject(App->scene->GetRootGameObject());
		aux->Load(object);
		go_list.push_back(aux);
	}

	//Order GOs with parent UUID
	for (uint i = 0; i < array_size; i++) {

		object = json_array_get_object(array, i);
		GameObject* parent = App->scene->GetGameObjectFromUUID(json_object_get_number(object, "Parent UUID"), App->scene->GetRootGameObject());
		if (parent) {

			parent->childs.push_back(go_list[i]);
			go_list[i]->SetParent(parent);

		}
	}

	//Finally load the Blackboard now that all GOs are created
	for (uint i = 0; i < go_list.size(); ++i) {

		GameObject* it = go_list[i];

		if (it) {

			for (uint j = 0; j < it->GetNumComp(); ++j) {

				if (it->GetComponentByIndex(j)->GetType() == Object_Type::CompGraphScript) {
					ComponentGraphScript* c = (ComponentGraphScript*)it->GetComponentByIndex(j);
					c->LoadBlackBoard();
				}
			}
		}
	}

	json_value_free(root_value);
}



void SceneSerialization::SaveGOs(GameObject* go, JSON_Array* array) {

	if (go->GetNumChilds() > 0) {

		for (std::vector<GameObject*>::const_iterator it = go->childs.begin(); it < go->childs.end(); it++) {


			(*it)->SaveArr(array);
			SaveGOs(*it, array);
		}

	}
}