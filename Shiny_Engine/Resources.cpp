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
