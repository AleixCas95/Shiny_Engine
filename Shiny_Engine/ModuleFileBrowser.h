#ifndef __ModuleFileBrowser_H__
#define __ModuleFileBrowser_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"

class ModuleFileBrowser : public Module
{
public:
	ModuleFileBrowser(Application* app, bool start_enabled = false);
	~ModuleFileBrowser();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void Draw(const char* title);

	bool active = true;


};

#endif 
