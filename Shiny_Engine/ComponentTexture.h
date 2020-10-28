#pragma once
#include "Application.h"
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

	


};

