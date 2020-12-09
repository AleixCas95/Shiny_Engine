#pragma once
#include "Globals.h"
#include <string>

enum class ResourceType
{
	None = 0,
	Mesh,
	Texture,
	Scene
};

class Resource
{
public:
	Resource(ResourceType type, const char* path);
	virtual ~Resource();

	virtual void Unload() {};

	bool operator==(Resource other);

public:
	std::string name;
	ResourceType type = ResourceType::None;
	int usage = 0;
};