#include "Component.h"
#include "GameObject.h"

Component::Component(Application* app_parent, GameObject* parent, Object_Type type) : App(app_parent), gameObject(parent), type(type) { uuid = pcg32_random(); }
{

}

Component::~Component()
{
}

bool Component::IsActive() const
{
	return active;
}

void Component::SetActive(bool active)
{
	this->active = active;
}

Object_Type Component::GetType() const
{
	return type;
}

GameObject* Component::GetGameObject() const
{
	return gameObject;
}
