#include "Application.h"
#include "Globals.h"
#include "ModuleFileBrowser.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

ModuleFileBrowser::ModuleFileBrowser(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}
ModuleFileBrowser::~ModuleFileBrowser()
{}

bool ModuleFileBrowser::Start()
{
	bool ret = true;
	return ret;
}
update_status ModuleFileBrowser::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleFileBrowser::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleFileBrowser::Draw(const char* title)
{

}