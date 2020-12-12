#pragma once
#include "Module.h"
#include "GameObject.h"
class ModuleGameObject :
	public Module
{
public:
	ModuleGameObject(Application* app, bool start_enabled = true);
	~ModuleGameObject();

	bool Init();

	void SaveScene(const char* name);

	void LoadScene(const char* name);

	void SaveGameObjects(JSON_Array*& parent, GameObject* current);

	update_status Update();

	GameObject* GetGO(unsigned int uuid);

public:
	GameObject* root = nullptr;

	std::list<GameObject*> gameObjects;

	std::list<GameObject*> gameObjectsToDelete;

	std::list<Component*> componentsToDelete;
};

