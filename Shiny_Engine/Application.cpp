#include "Application.h"
#include "pcg/pcg_basic.h"
#include <time.h>

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
	hierarchy = new ModuleHierarchy(this);
	fileB = new ModuleFileBrowser(this);
	files = new ModuleFiles(this);
	mouse = new ModuleMousePicking(this);
	module_time = new Time(this);
	resources = new ModuleResources(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	//UI
	
	/*AddModule(console);
	AddModule(hardware);
	AddModule(config);
	AddModule(about);
	AddModule(inspector);
	AddModule(hierarchy);*/
	AddModule(fileB);
	AddModule(module_time);
	AddModule(mouse);
	
	// Scenes
	AddModule(files);
	AddModule(scene);
	AddModule(fbx);
	AddModule(gobject);
	AddModule(resources);

	AddModule(renderer3D);
	AddModule(gui);



	

}

Application::~Application()
{
	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		delete(*item);
		list_modules.erase(item);
		item = list_modules.begin();
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}

	pcg32_random_t RNG1;
	pcg32_srandom(time(NULL), (intptr_t)&RNG1);

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	fps_log.push_back(1 / dt);
	if (fps_log.size() > 75)
	{
		fps_log.erase(fps_log.begin());
	}

	ms_log.push_back(dt * 1000);
	if (ms_log.size() > 75)
	{
		ms_log.erase(ms_log.begin());
	}
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	/*if (!renderer3D->vsync && toCap)
	{
		float dt = dt * 1000.0f;
		float toVsync = dt;

		if (capFrames > 0)
			toVsync = 1000.0f / capFrames;

		if (dt < toVsync)
			SDL_Delay(toVsync - dt);
	}*/
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}