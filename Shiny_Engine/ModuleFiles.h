#ifndef MODULE_FILES_H
#define MODULE_FILES_H

#include "Module.h"
#include "Globals.h"
#include <map>
#include <string.h>

class Files;
class ModuleFiles : public Module
{
public:
	ModuleFiles(Application* app, bool start_enabled = true);
	~ModuleFiles();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	uint SearchFiles(std::string path);

public:
	std::map<uint, Files*> resources;
};

#endif 