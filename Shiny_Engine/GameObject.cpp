#include "GameObject.h"
#include "pcg/pcg_basic.h"
#include "Application.h"

#include <list>

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

GameObject* GameObject::GetParent() const {
	return parent;
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
	if (type == CompGraphScript && script)
	{
		return script;
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

ComponentGraphScript* GameObject::GetCompScript(uint num) const
{
	for (int i = 0; i < components.size(); ++i) {
		if (components_vec[i]->GetType() == Object_Type::CompGraphScript) {
			ComponentGraphScript* gs = (ComponentGraphScript*)components_vec[i];
			if (gs->GetCompSriptNum() == num)
				return gs;
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

void GameObject::SetName(const char* new_name) {

	strcpy_s(charname, 100, new_name);
	LOG("name = %s", charname);
}

uint GameObject::GetNumComp() const
{
	return components_vec.size();
}

Component* GameObject::GetComponentByIndex(uint i) const
{
	return components_vec[i];
}


Component* GameObject::CreateComponent(Object_Type type)
{
	Component* component = nullptr;

	switch (type) {
	case Object_Type::CompTransform:
		if (transform == nullptr) {
			transform = new ComponentTransform(App, this);
			component = transform;
		}
		break;
	case Object_Type::CompMesh:
		if (GetComponent(Object_Type::CompMesh) == nullptr)
			component = new ComponentMesh(App, this);
		else
			return nullptr;
		break;
	case Object_Type::CompCamera:
		if (GetComponent(CompCamera) == nullptr)
			component = new ComponentCamera(App, this);
		else
			return nullptr;
		break;
	case Object_Type::CompGraphScript:
	{
		uint script_num = 1;
		for (int i = 0; i < components_vec.size(); ++i) {
			if (components_vec[i]->GetType() == Object_Type::CompGraphScript)
				script_num++;
		}
		component = new ComponentGraphScript(App, this, script_num);
	}
	break;
	}

	if (component)
		components.push_back(component);

	return component;
}

const char* GameObject::GetName() const
{
	return charname;
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

void GameObject::UpdateBoundingBox() {

	boundingBox.SetNegativeInfinity();

	ComponentMesh* mesh = (ComponentMesh*)GetComponent(CompMesh);
	if (mesh && mesh->uuid_mesh != 0) {
		ResourceMesh* res_mesh = (ResourceMesh*)App->resources->Get(mesh->uuid_mesh);
		boundingBox.Enclose((const math::float3*)res_mesh->vertex.data, res_mesh->num_vertex);
	}

	if (transform) {

		obb.SetFrom(boundingBox);
		obb.Transform(transform->GetMatrix());
		if (obb.IsFinite()) {
			boundingBox = obb.MinimalEnclosingAABB();
		}
	}

	//for (uint i = 0; i < childs.size(); ++i) {

	//	childs[i]->UpdateBoundingBox();
	//}

}