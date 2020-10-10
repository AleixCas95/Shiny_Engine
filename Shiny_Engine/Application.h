#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleGUI.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	//ModulePlayer* player;
	ModuleGUI* gui;
	
	char* app_name;
	char* organization;

private:

	Timer	ms_timer;
	float	dt = 0.0f;
	float last_FPS = 0.0f;
	float last_ms = 0.0f;
	p2List<Module*> list_modules;

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