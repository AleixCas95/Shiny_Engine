#ifndef __Panel_About_H__
#define __Panel_About_H__


#include "Globals.h"
#include "imGUI\imgui.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_cpuinfo.h"
#include "SDL\include\SDL_opengl.h"

class ModuleAbout : public Module
{
public:
	ModuleAbout(Application* app, bool start_enabled = true);
	virtual ~ModuleAbout();

	void Draw() override;

};

#endif 
