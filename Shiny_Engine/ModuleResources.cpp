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
	string direction = GetDirection(type, uuid, path);

	ifstream loadFile(direction.c_str(), ios::out | ios::binary);

	char* ret = nullptr;

	if (loadFile.is_open())
	{
		loadFile.seekg(0, loadFile.end);
		uint size = loadFile.tellg();

		ret = new char[size];

		loadFile.seekg(0, loadFile.beg);

		loadFile.read(ret, size);

		loadFile.close();
	}
	return ret;
}

string ModuleResources::GetDirection(ResourceType type, uint uuid, const char* path)
{
	string filePath = "Library/";
	switch (type)
	{
	case ResourceType::Mesh:
	{
		filePath += "Models/";
		filePath += to_string(uuid);
		filePath += ".dmnd";
	}
	break;
	case ResourceType::Texture:
	{
		filePath += "Textures/";
		string name(path);
		uint initialPos = name.find_last_of("\\") + 1;
		uint finalPos = name.find_last_of(".") + 1;
		filePath += name.substr(initialPos, (finalPos - initialPos)) + "dds";
	}
	break;

	case ResourceType::Scene:
	{
		filePath = "Assets/Scenes/";
		string name(path);
		uint initialPos = name.find_last_of("\\") + 1;
		uint finalPos = name.find_last_of(".");
		filePath += name.substr(initialPos, (finalPos - initialPos)) + ".json";
	}
	break;
	default:
		break;
	}

	return filePath;
}

Resource* ModuleResources::GetResource(ResourceType type, const char* path)
{
	for (std::list<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it)->type == type)
		{
			if (strcmp((*it)->name.c_str(), path) == 0)
				return *it;
		}
	}
	return nullptr;
}

void ModuleResources::AddResource(Resource* resource)
{
	for (std::list<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (&resource == &*it)
			return;
	}
	resources.push_back(resource);
	ResourceUsageIncreased(resource);
}

void ModuleResources::ResourceUsageIncreased(Resource* resource)
{
	resource->usage++;
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