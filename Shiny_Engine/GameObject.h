#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.h"
#include "ComponentTransform.h"
#include "Globals.h"

#include <string>
#include <list>
class Application;

class GameObject
{
public:
	Application* App;
	GameObject(Application* app_parent,GameObject* parent, const char* name = nullptr, bool addToList = true);
	~GameObject();

	bool HasComponent(Object_Type type);

	Component* GetComponent(Object_Type type);

	void Save(JSON_Object* parent);

	void Load(JSON_Object* info);

	bool SetParent(GameObject* parent);
public:
	bool active = true;
	std::string name = "gameObject";
	std::list<Component*> components;
	ComponentTransform* transform = nullptr;
	GameObject* parent = nullptr;
	std::list<GameObject*> childs;

	AABB originalBoundingBox;
	AABB boundingBox;


	bool isStatic = false;

	unsigned int uuid = 0u;

	unsigned int parentUUID = 0u;
};

#endif // !GAMEOBJECT_H