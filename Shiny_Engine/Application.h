#ifndef __Application__H__
#define __Application__H__

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
#include "ModuleShapes.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "ModuleInspector.h"
#include "ModuleHierarchy.h"
#include "ModuleFiles.h"
#include "Component.h"
#include "ModuleMousePicking.h"
#include "ModuleFileBrowser.h"
#include "ModuleTime.h"
#include "ModuleResources.h"
#include "ModuleAssets.h"
#include "SceneSerialization.h"

#include <string>
#include <list>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;

	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;
	ModuleConfiguration* config;
	ModuleHardware* hardware;
	ModuleConsole* console;
	ModuleAbout* about;
	ModuleInspector* inspector;
	ModuleHierarchy* hierarchy;
	ModuleShapes* shape;
	ModuleFileBrowser* fileB;
	ModuleFBX* fbx;
	ModuleGameObject* gobject;
	ModuleScene* scene;
	ModuleFileSystem* files;
	ModuleMousePicking* mouse;
	ModuleResources* resources;
	ModuleAssets* assets;
	SceneSerialization* scene_serialization;

	Time* module_time;



	string organization_name;
	string app_name;


private:

	Timer	ms_timer;
	float	dt = 0.0f;
	float last_FPS = 0.0f;
	float last_ms = 0.0f;
	std::list<Module*> list_modules;
	LCG lcg;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetMS();
	float GetFPS();

	u32 GenerateUUID();

	JSON_Value* JSONconfig = nullptr;
	JSON_Object* JSONconfig_obj = nullptr;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;


#endif