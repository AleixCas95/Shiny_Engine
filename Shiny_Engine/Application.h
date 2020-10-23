#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "ModuleGUI.h"
#include "ModuleFBX.h"
#include "ModuleHardware.h"
#include "ModuleConfiguration.h"
#include "ModuleConsole.h"
//#include "ModuleGeometry.h"
#include <list>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	//ModulePlayer* player;
	ModuleGUI* gui;
	ModuleFBX* fbx;
	ModuleConfiguration* config;
	ModuleHardware* hardware;
	ModuleConsole* console;
	//ModuleGeometry* geometry;

	char* app_name;
	char* organization;

private:

	Timer	ms_timer;
	float	dt = 0.0f;
	float last_FPS = 0.0f;
	float last_ms = 0.0f;
	std::list<Module*> list_modules;


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetMS();
	float GetFPS();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};