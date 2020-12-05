#pragma once
#include "Globals.h"
#include "parson/parson.h"
class Application;
class GameObject;

enum Object_Type
{
	None = -1,

	CompTransform = 0,
	CompMesh,
	CompTexture,
	CompCamera,
	CompLight
};

class Component 
{
public:

	Application* App;

	Component(Application* papaito ,GameObject* parent, Object_Type type) : App(papaito),gameObject(parent), type(type) {}
	~Component() {}

	virtual void Save(JSON_Object* parent) {}

	virtual void Inspector() {}
public:
	bool active = true;
	GameObject* gameObject = nullptr;
	Object_Type type;
};