#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentGraphScript.h"
#include "Globals.h"

#include <string>
#include <list>
#include<vector>
class Application;

class GameObject
{
public:
	Application* App;
	GameObject(Application* app_parent, GameObject* parent, const char* name = nullptr, bool addToList = true);
	~GameObject();

	void RealDelete();

	bool HasComponent(Object_Type type);

	Component* CreateComponent(Object_Type type);
	Component* GetComponent(Object_Type type);
	ComponentGraphScript* GetCompScript(uint num) const;
	Component* GetComponentByIndex(uint i) const;

	GameObject* GetParent() const;

	void Save(JSON_Object* parent);
	void SaveArr(JSON_Array* go_array) const;

	void Load(JSON_Object* info);

	bool SetParent(GameObject* parent);
	void SetName(const char* new_name);
	uint GetNumComp()const;
	uint GetNumChilds() const;
	const char* GetName()const;
	void SetActive(bool active);
	bool IsActive() const;
	void UpdateBoundingBox();

	void DrawComponentsInspector();

public:

	std::string name = "gameObject";
	char charname[100];

	std::list<Component*> components;
	std::vector<Component*> components_vec;

	ComponentTransform* transform = nullptr;
	ComponentGraphScript* script = nullptr;
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	AABB originalBoundingBox;
	AABB boundingBox;
	math::OBB obb;

	bool active = true;
	bool isStatic = false;
	bool focused = false;

	unsigned int uuid = 0u;

	unsigned int parentUUID = 0u;
};

#endif // !GAMEOBJECT_H