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
	//update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	bool showShapes = false;
	bool showHierarchy = true;
	bool showInspector = true;
	bool showConsole = true;
	bool showCamera = false;
	bool saveScenePopup = false;
	bool loadScenePopup = false;


private:
	bool configActive = true;
	//bool hirearchyActive = true;
	//bool inspectorActive = true;
	//bool configActive = true;

};