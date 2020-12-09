#include "ResourcesTexture.h"
#include "Glew/include/glew.h"

ResourceTexture::ResourceTexture(const char* path) : Resource(ResourceType::Texture, path)
{
}

ResourceTexture::~ResourceTexture()
{
	glDeleteTextures(1, (GLuint*) & (id));
}

void ResourceTexture::Unload()
{
	
}