#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include <vector>

class UiMainMenu;

class ModuleGUI: public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	UiMainMenu* mainMenu = nullptr;
	ImVec4 yellow_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	SDL_version version;
	int CPU_Cache = 0;
	int CPU_Count = 0;
	int ram = 0;

	int total_memory;
	int memory_usage = 0;
	int dedicated_memory = 0;
	int available_memory = 0;

	bool custom = false;
	bool normal = true;


private:
	std::vector<float> vector_ms;
	std::vector<float> vector_fps;
};

#endif 