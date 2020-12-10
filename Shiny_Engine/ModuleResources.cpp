#include "ModuleResources.h"
#include <fstream>
#include <iostream>
using namespace std;


ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{
	CreateDirectory("Library", NULL);
	CreateDirectory("Library/Models", NULL);
	CreateDirectory("Library/Textures", NULL);

	return true;
}

