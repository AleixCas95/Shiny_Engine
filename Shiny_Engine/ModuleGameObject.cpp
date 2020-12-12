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