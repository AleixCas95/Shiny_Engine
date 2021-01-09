#include "GameObject.h"
#include "pcg/pcg_basic.h"
#include "Application.h"



using namespace std;


GameObject::GameObject(Application* app_parent, GameObject* parent, const char* name, bool addToList)
{
	App = app_parent;

	this->parent = parent;
	if (parent)
		parent->childs.push_back(this);
	if (name)
		this->name = name;

	if (addToList)
		App->gobject->gameObjects.push_back(this);

	transform = new ComponentTransform(App, this);

	uuid = pcg32_random();

	originalBoundingBox.SetNegativeInfinity();
	boundingBox.SetNegativeInfinity();
}


GameObject::~GameObject()
{
	delete transform;
	transform = nullptr;
}

void GameObject::RealDelete()
{
	for (auto comp : components)
	{
		delete comp;
		comp = nullptr;
	}
	components.clear();

	childs.clear();

	parent = nullptr;
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


void GameObject::Save(JSON_Object* parent)
{
	json_object_set_string(parent, "Name", name.data());
	json_object_set_number(parent, "UUID", uuid);

	if (this->parent)
	{
		json_object_set_number(parent, "Parent UUID", this->parent->uuid);
	}

	json_object_set_boolean(parent, "Active", active);
	json_object_set_boolean(parent, "Static", isStatic);

	JSON_Value* componentsValue = json_value_init_array();
	JSON_Array* componentsObj = json_value_get_array(componentsValue);

	json_object_set_value(parent, "Components", componentsValue);

	JSON_Value* transformValue = json_value_init_object();
	JSON_Object* transformObj = json_value_get_object(transformValue);

	transform->Save(transformObj);

	json_array_append_value(componentsObj, transformValue);

	for (auto component : components)
	{
		JSON_Value* componentValue = json_value_init_object();
		JSON_Object* componentObj = json_value_get_object(componentValue);

		component->Save(componentObj);

		json_array_append_value(componentsObj, componentValue);
	}
}

void GameObject::Load(JSON_Object* info)
{
	uuid = json_object_get_number(info, "UUID");

	parentUUID = json_object_get_number(info, "Parent UUID");

	active = json_object_get_boolean(info, "Active");
	isStatic = json_object_get_boolean(info, "Static");

	JSON_Array* objComps = json_object_get_array(info, "Components");

	int componentsInObj = json_array_get_count(objComps);

	for (int i = 0; i < componentsInObj; ++i)
	{
		JSON_Object* comp = json_array_get_object(objComps, i);

		Object_Type compType = (Object_Type)(int)json_object_get_number(comp, "Type");

		switch (compType)
		{
		case None:
			break;
		case CompTransform:
		{
			transform->Load(comp);
		}
		break;
		case CompMesh:
		{
			ComponentMesh* mesh = new ComponentMesh(App, this);
			mesh->Load(comp);
			originalBoundingBox.Enclose((float3*)mesh->mesh->vertex.data, mesh->mesh->vertex.size / 3);
			boundingBox = originalBoundingBox;
		}
		break;
		case CompTexture:
		{
			ComponentTexture* tex = new ComponentTexture(App, this);
			tex->Load(comp);
		}
		break;
		case CompCamera:
		{
			ComponentCamera* cam = new ComponentCamera(App, this);
			cam->Load(comp);
		}
		break;
		case CompGraphScript:
		{
			uint c = 0;
			ComponentGraphScript* gra = new ComponentGraphScript(App, this, c);
			gra->Load(comp);
		}
		break;
		default:
			break;
		}

	}
}

bool GameObject::SetParent(GameObject* parent)
{
	bool ret = false;
	if (parent)
	{
		if (this->parent)
		{
			this->parent->childs.remove(this);
		}

		for (auto child : childs)
		{
			if (parent == child)
			{
				this->childs.remove(child);
				parent->SetParent(this->parent);
			}
		}
		this->parent = parent;
		parent->childs.push_back(this);
		ret = true;
	}

	return ret;
}

const char* GameObject::GetName() const
{
	return namechar
		;
}

void GameObject::SetActive(bool active)
{
	this->active = active;
}

bool GameObject::IsActive() const
{
	return active;
}