#pragma once
#include "Globals.h"
#include <string>

enum class ResourceType
{
	None = 0,
	Mesh,
	Texture,
	Script,
	Scene
};

class Resource
{
public:
	Resource(ResourceType type, const char* path);
	virtual ~Resource();

	virtual void Unload() {};
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	const char* GetFile() const;
	bool operator==(Resource other);

public:
	std::string name;
	std::string file;
	scriptType uuid_1 = 0;
	uint uuid = 0u;
	ResourceType type = ResourceType::None;
	int usage = 0;
	int loaded = 0;
};