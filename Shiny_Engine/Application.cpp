#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);
	fbx = new ModuleFBX(this);
	config = new ModuleConfiguration(this);
	hardware = new ModuleHardware(this);
	console = new ModuleConsole(this);
	about = new ModuleAbout(this);
	shape = new ModuleShapes(this);
	gobject = new ModuleGameObject(this);
	inspector = new ModuleInspector(this);
	

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(gobject);
	AddModule(shape);

	//UI
	AddModule(console);
	AddModule(hardware);
	AddModule(config);
	AddModule(about);
	AddModule(inspector);

	// Scenes
	AddModule(scene);
	AddModule(fbx);

	AddModule(renderer3D);
	AddModule(gui);
}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		delete item._Ptr->_Myval;
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->Start();
		item++;
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->PostUpdate(dt);
		item++;
	}


	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.end();

	while (item != list_modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->CleanUp();
		item++;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}


float Application::GetMS()
{
	return last_ms;
}

float Application::GetFPS()
{
	return last_FPS;
}