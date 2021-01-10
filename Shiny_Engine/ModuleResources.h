#pragma once
#include "Module.h"
#include <string>
#include "ComponentTexture.h"
#include "ComponentGraphScript.h"
#include "Resources.h"
#include "Globals.h"
#include <list>
#include <map>

class Resource;
class ResourceMesh;
//enum ResourceType;

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init();

	void SaveFile(uint size, char* output_file, ResourceType type, uint uuid, const char* path = nullptr);

	char* LoadFile(const char* path, ResourceType type, uint uuid);

	std::string GetDirection(ResourceType type, uint uuid, const char* path = nullptr);

	Resource* GetResource(ResourceType type, const char* path);
	Resource* Get(scriptType uid);

	void AddResource(Resource* resource);

	void ResourceUsageIncreased(Resource* resource);

	void ResourceUsageDecreased(Resource* resource);

	scriptType GetUIDFromMeta(std::string path_no_meta) const;
	ResourceMesh* GetPrimitive(PrimitiveType type);

public:
	std::map<scriptType, std::string> names;
	std::list<Resource*> resources;
	std::map<scriptType, Resource*> resources_map;

	
};