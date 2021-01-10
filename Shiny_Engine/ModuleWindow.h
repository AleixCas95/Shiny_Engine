#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetFullScreen(bool fullscreen);
	void SetWindowed(bool borderless);
	void SetWindowFullDesktop();
	void GetWinSize(int& x, int& y) const;



public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	Uint32 flags;
	bool fullscreen = false;
	bool windowed = false;
	bool full_desktop = false;

	SDL_Surface* icon = nullptr;

	int width = 0;
	int height = 0;
	int scale = 0;

	float brightness = 0;

};

#endif // __ModuleWindow_H__