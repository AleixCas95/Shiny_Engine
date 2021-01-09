#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentGraphScript.h"
#include "Globals.h"

#include <string>
#include <list>
class Application;

class GameObject
{
public:
	Application* App;
	GameObject(Application* app_parent, GameObject* parent, const char* name = nullptr, bool addToList = true);
	~GameObject();

	void RealDelete();

	bool HasComponent(Object_Type type);

	Component* GetComponent(Object_Type type);

	void Save(JSON_Object* parent);

	void Load(JSON_Object* info);

	bool SetParent(GameObject* parent);
	const char* GetName()const;
	void SetActive(bool active);
	bool IsActive() const;

public:

	std::string name = "gameObject";
	char namechar[50];
	std::list<Component*> components;
	ComponentTransform* transform = nullptr;
	GameObject* parent = nullptr;
	std::list<GameObject*> childs;

	AABB originalBoundingBox;
	AABB boundingBox;

	bool active = true;
	bool isStatic = false;

	unsigned int uuid = 0u;

	unsigned int parentUUID = 0u;
};

#endif // !GAMEOBJECT_H