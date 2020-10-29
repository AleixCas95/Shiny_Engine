#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "parson\parson.h"
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
#include "ModuleAbout.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
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
	ModuleAbout* about;
	ModuleGameObject* gobject;
	//ModuleGeometry* geometry;



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

	JSON_Value* JSONconfig = nullptr;
	JSON_Object* JSONconfig_obj = nullptr;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};