#ifndef MODULEUI_H
#define MODULEUI_H

#include "Module.h"
#include "Globals.h"
#include <vector>

class Panel;
//class Module_Configuration;
class PanelAbout;
class PanelConsole;


class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

private:
	bool configActive = true;
	std::vector<Panel*> panels;

public:
	//Module_Configuration* config = nullptr;
	PanelAbout* about = nullptr;
	PanelConsole* console = nullptr;
};

#endif // !MODULEUI_H