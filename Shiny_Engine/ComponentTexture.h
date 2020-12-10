#pragma once
#include "Application.h"
//#include "ModuleImport.h"
#include "Component.h"
#include "GameObject.h"
#include "ModuleFBX.h"
#include <string>

class ComponentTexture :
	public Component
{
public:
	ComponentTexture(Application* app_parent, GameObject* parent);
	~ComponentTexture();

	void Inspector();

	inline unsigned int GetID() { return checkers == false ? tex_id : 0u; }


	void Save(JSON_Object* parent);

	void Load(JSON_Object* parent);

public:
	unsigned int tex_id = 0u;

	ResourceTexture* RTexture;

	std::string name;

	std::string path;

	bool print = true;

	bool checkers = false;
};

