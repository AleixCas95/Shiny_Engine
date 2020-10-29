#pragma once
#include "Module.h"
#include "GameObject.h"
class ModuleGameObject :
	public Module
{
public:
	ModuleGameObject(Application* app, bool start_enabled = true);
	~ModuleGameObject();

public:
	GameObject* root = nullptr;
};

