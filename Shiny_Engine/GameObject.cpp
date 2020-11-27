#include "GameObject.h"
#include "pcg/pcg_basic.h"
#include "Application.h"



using namespace std;


GameObject::GameObject(Application* papaito,GameObject* parent, const char* name)
{
	App = papaito;
	this->parent = parent;
	if (parent)
		parent->childs.push_back(this);
	if (name)
		this->name = name;

	transform = new ComponentTransform(App,this);

	//uuid = pcg32_random();

	//components.push_back((Component*)transform);
}


GameObject::~GameObject()
{
}

bool GameObject::HasComponent(Object_Type type)
{
	if (type == CompTransform && transform)
	{
		return true;
	}
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
		{
			return true;
		}
	}
	return false;
}

Component* GameObject::GetComponent(Object_Type type)
{
	if (type == CompTransform && transform)
	{
		return transform;
	}
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
		{
			return (*it);
		}
	}
	return nullptr;
}
