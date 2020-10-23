#ifndef __Module_Configuration_H__
#define __Module_Configuration_H__

#include "Application.h"
#include "imGUI\imgui.h"

#define GRAPH_ARRAY_SIZE 70

class Module_Configuration : public Module
{
public:
	Module_Configuration(Application* app, bool start_enabled = true);
	~Module_Configuration();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddLog(const char*, ...) IM_FMTARGS(2);
	int GetFPS();

	void Draw(const char* title);

	bool active = false;
private:

	int rand = 0;
	int width = 1280;
	int height = 1024;
	float brightness = 1.0;
	int fpsCap = 0;
	float lightColour[4];
	float lightPos[3];
	bool lightOn = true;
	float			fps_array[GRAPH_ARRAY_SIZE];
	float			ms_array[GRAPH_ARRAY_SIZE];
	float			mem_array[GRAPH_ARRAY_SIZE];
	

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static char* Strdup(const char* str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
};

#endif 
