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

void ModuleResources::SaveFile(uint size, char* output_file, ResourceType type, uint uuid, const char* path)
{

	string direction = GetDirection(type, uuid, path);

	ofstream file(direction.c_str(), ios::out | ios::binary);

	if (file.is_open())
	{
		file.write(output_file, size);
		file.close();
	}
}

char* ModuleResources::LoadFile(const char* path, ResourceType type, uint uuid)
{
	
}

string ModuleResources::GetDirection(ResourceType type, uint uuid, const char* path)
{
	string filePath = "Library/";

	return filePath;
}

void ModuleResources::ResourceUsageDecreased(Resource* resource)
{
	resource->usage--;
	if (resource->usage <= 0)
	{
		///resource->Unload();
		resources.remove(resource);
		delete resource;
	}
}