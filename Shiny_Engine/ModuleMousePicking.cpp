#include "ModuleMousePicking.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleGameObject.h"


ModuleMousePicking::ModuleMousePicking(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleMousePicking::~ModuleMousePicking()
{
}

update_status ModuleMousePicking::Update()
{
	return UPDATE_CONTINUE;
}