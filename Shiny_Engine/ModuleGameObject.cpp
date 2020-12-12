#include "ModuleGameObject.h"
#include "Application.h"


ModuleGameObject::ModuleGameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}


ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Init()
{
	root = new GameObject(App, nullptr, "Root");
	return true;
}

void ModuleGameObject::SaveScene(const char* name)
{
	if (root)
	{
		JSON_Value* rootValue = json_value_init_array();
		JSON_Array* rootObj = json_value_get_array(rootValue);

		json_object_set_value(nullptr, "Scene", rootValue);

		SaveGameObjects(rootObj, root);

		int size = json_serialization_size_pretty(rootValue);

		char* buffer = new char[size];

		json_serialize_to_buffer_pretty(rootValue, buffer, size);

		App->resources->SaveFile(size, buffer, ResourceType::Scene, 0, name);
	}
}

void ModuleGameObject::LoadScene(const char* name)
{
	JSON_Value* scene = json_parse_file(name);
	if (json_value_get_type(scene) == JSONArray)
	{
		
		for (auto gameObj : gameObjects)
		{
			gameObj->RealDelete();
			delete gameObj;
		}

		gameObjects.clear();
		App->scene->current_object = nullptr;
		App->renderer3D->mesh_list.clear();


	}

	JSON_Array* objArray = json_value_get_array(scene);

	int objectsInScene = json_array_get_count(objArray);

	std::list<GameObject*> goInNewScene;

	for (int i = 0; i < objectsInScene; ++i)
	{
		JSON_Object* currentGO = json_array_get_object(objArray, i);

		GameObject* newGO = new GameObject(App, nullptr, json_object_get_string(currentGO, "Name"));
		newGO->Load(currentGO);

		goInNewScene.push_back(newGO);
	}

	for (auto obj : goInNewScene)
	{
		if (!obj->SetParent(GetGO(obj->parentUUID)))
		{
			root = obj;
		}
	}
	root->transform->UpdateBoundingBox();
}
}

void ModuleGameObject::SaveGameObjects(JSON_Array*& parent, GameObject* current)
{
	
}

update_status ModuleGameObject::Update()
{
	
}

GameObject* ModuleGameObject::GetGO(unsigned int uuid)
{
	
	return nullptr;
}