#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();
	bool showShapes = false;
	bool showHierarchy = false;
	bool showInspector = false;

private:
	bool configActive = true;

};