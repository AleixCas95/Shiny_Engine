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
	ComponentTexture(GameObject* parent);
	~ComponentTexture();

	void Inspector();

	inline unsigned int GetID() { return checkers == false ? tex_id : 0u; }

public:
	unsigned int tex_id = 0u;

	std::string name;

	std::string path;

	bool print = true;

	bool checkers = false;
};

