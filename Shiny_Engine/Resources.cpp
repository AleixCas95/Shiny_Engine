#include "Resources.h"
#include "Application.h"

Resource::Resource(ResourceType type, const char* path) : type(type), name(path)
{
}

Resource::~Resource()
{
}

bool Resource::operator==(Resource other)
{
	if (this->type == other.type)
	{
		if (strcmp(this->name.data(), other.name.data()) == 0)
			return true;
	}
	return false;
}

bool Resource::IsLoadedToMemory() const
{
	if (loaded == 0)
		return false;
	else
		return true;
}

bool Resource::LoadToMemory() //TODO change maybe
{
	bool ret = true;
	/*if (!IsLoadedToMemory())
		ret = LoadInMemory();*/

	loaded++;

	return ret;
}

const char* Resource::GetFile() const
{
	return file.c_str();
}

