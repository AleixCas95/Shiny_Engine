#include "Globals.h"
#include "Application.h"
#include "ModuleFiles.h"

ModuleFiles::ModuleFiles(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleFiles::~ModuleFiles()
{}

bool ModuleFiles::Init()
{
	return true;
}

update_status ModuleFiles::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFiles::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFiles::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleFiles::CleanUp()
{
	return true;
}

uint ModuleFiles::SearchFiles(std::string path)
{
	
	return 0;
}
