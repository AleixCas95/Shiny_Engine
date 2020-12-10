#pragma once
#include "Application.h"
//#include "ModuleImport.h"
#include "Component.h"
#include "GameObject.h"
#include "ResourcesTexture.h"
#include <string>

class ComponentTexture :
	public Component
{
public:
	ComponentTexture(Application* app_parent, GameObject* parent);
	~ComponentTexture();

	void Inspector();

	unsigned int GetID();


	void Save(JSON_Object* parent);

	void Load(JSON_Object* parent);

public:

	ResourceTexture* RTexture;

	std::string name;

	std::string path;

	bool print = true;

	bool checkers = false;
};

