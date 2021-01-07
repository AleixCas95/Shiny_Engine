#pragma once
#include "Globals.h"
#include "parson/parson.h"
#include "pcg/pcg_basic.h"

class Application;
class GameObject;

enum Object_Type
{
	None = -1,

	CompTransform = 0,
	CompMesh,
	CompTexture,
	CompCamera,
	CompLight,
	CompGraphScript
};

class Component 
{
public:

	Application* App;

	Component(Application* app_parent,GameObject* parent, Object_Type type) : App(app_parent),gameObject(parent), type(type) { uuid = pcg32_random(); }
	~Component() {}

	virtual void Save(JSON_Object* parent) {}

	virtual void Inspector() {}
public:
	bool active = true;
	GameObject* gameObject = nullptr;
	Object_Type type;

	unsigned int uuid = 0u;
};