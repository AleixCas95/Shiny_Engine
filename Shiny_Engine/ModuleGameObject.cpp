#include "ModuleGameObject.h"



ModuleGameObject::ModuleGameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject(nullptr, "Root");
}


ModuleGameObject::~ModuleGameObject()
{
}
