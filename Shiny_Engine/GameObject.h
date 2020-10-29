#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "Component.h"
#include "ComponentTransform.h"

#include <string>
#include <list>

class GameObject
{
public:
	GameObject(GameObject* parent, const char* name = nullptr);
	~GameObject();

	bool HasComponent(Object_Type type);

	Component* GetComponent(Object_Type type);

public:
	bool active = true;
	std::string name = "gameObject";
	std::list<Component*> components;
	ComponentTransform* transform = nullptr;
	GameObject* parent = nullptr;
	std::list<GameObject*> childs;

	unsigned int uuid = 0u;
};

#endif // !GAMEOBJECT_H