#include "Application.h"
#include "ModuleFBX.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "MathGeoLib\Geometry\AABB.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

using namespace std;

ModuleFBX::ModuleFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleFBX::~ModuleFBX()
{}

bool ModuleFBX::Start()
{
	bool ret = true;

	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	struct aiLogStream stream = aiLogStream();
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
	
}

update_status ModuleFBX::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleFBX::ClearMeshes()
{
	
}

bool ModuleFBX::CleanUp()
{
	

	return true;
}

bool ModuleFBX::LoadFBX(const char* path)
{
	
	return false;
}

void ModuleFBX::LoadModel(const aiScene* scene, aiNode* node, const char* path)
{
}