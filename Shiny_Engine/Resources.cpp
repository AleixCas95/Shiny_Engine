#include "Resources.h"
#include "Application.h"

Resource::Resource(scriptType uuid, ResourceType type) : uuid(uuid), type(type)
{
}

Resource::~Resource()
{
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

	loaded++;

	return ret;
}

const char* Resource::GetFile() const
{
	return file.c_str();
}

bool Resource::UnloadToMemory()
{
	bool ret = true;
	if (loaded > 0)
		loaded--;

	if (loaded == 0)
		ret = UnloadInMemory();

	return ret;
}

scriptType Resource::GetUID() const
{
	return uuid;
}