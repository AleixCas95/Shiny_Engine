#include "Globals.h"

#include <random>

// ~~ FUNCTIONS ~~
bool FileExist(const std::string& name)
{
	if (FILE* file = fopen(name.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	else
	{
		return false;
	}
}

std::string GetFileNameFromPath(const char* path)
{
	std::string filename = path;

	
	return filename;
}

std::string GetFileNameWithExtension(const char* path)
{
	std::string filename = path;

	return filename;
}

uint GenRandomNumber()
{
	uint number = 0;
	

	return number;
}

std::vector<std::string> ReadAllFilesOnPath(const char* path, const char* extension)
{
	std::vector<std::string> paths;
	
	return paths;
}
