#include "Application.h"
#include "ResourcesTexture.h"
#include <string>
#include "Glew/include/glew.h"


ResourceTexture::ResourceTexture(scriptType uuid) : Resource(uuid, ResourceType::Texture)
{
}


ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	bool ret = App->fbx->LoadFBX(std::to_string(uuid).c_str());

	return ret;
}

bool ResourceTexture::UnloadInMemory()
{
	glDeleteTextures(1, (GLuint*)&tex_id);

	tex_id = 0;
	tex_width = 0;
	tex_height = 0;

	return true;
}
