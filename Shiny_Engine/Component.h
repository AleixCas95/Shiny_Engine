#pragma once
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
	Component(GameObject* parent, Object_Type type) : gameObject(parent), type(type) {}
	~Component() {}

	virtual void Inspector() {}
public:
	bool active = true;
	GameObject* gameObject = nullptr;
	Object_Type type;
};