#include "ModuleGameObject.h"



ModuleGameObject::ModuleGameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject(App,nullptr, "Root");
}


ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Init()
{
	
	return true;
}

void ModuleGameObject::SaveScene(const char* name)
{
	
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