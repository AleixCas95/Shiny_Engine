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
	//update_status PostUpdate(float dt);
	bool CleanUp();

public:
	UiMainMenu* mainMenu = nullptr;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	SDL_version version;
	int CPU_Cache = 0;
	int CPU_Count = 0;
	int ram = 0;



private:
	std::vector<float> vector_ms;
	std::vector<float> vector_fps;

};

#endif 