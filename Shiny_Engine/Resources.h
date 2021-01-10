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
	Resource(scriptType uuid, ResourceType type);
	virtual ~Resource();

	virtual void Unload() {};
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	bool UnloadToMemory();
	scriptType GetUID() const;
	const char* GetFile() const;
	//bool operator==(Resource other);

public:
	std::string name;
	std::string file;
	scriptType uuid_1 = 0;
	uint uuid = 0u;
	ResourceType type = ResourceType::None;
	int usage = 0;
	int loaded = 0;

	virtual bool UnloadInMemory() = 0;
	virtual bool LoadInMemory() = 0;
};