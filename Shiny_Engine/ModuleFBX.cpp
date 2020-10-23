#include "Application.h"
#include "ModuleFBX.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "MathGeoLib\Geometry\AABB.h"
#include <math.h>

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

using namespace std;

void myCallback(const char* msg, char* userData) {
	LOG("%s", msg);
}

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

void ModuleFBX::ClearMeshes()
{
	for (int i = meshes.size() - 1; meshes.size() != 0; i--)
	{
		delete[] meshes[i].indices;
		delete[] meshes[i].vertices;
		delete[] meshes[i].uvs;
		delete[] meshes[i].normals;
		meshes.pop_back();
	}

	//Geometry
	delete data.indices;
	delete data.vertices;

	//Texture
	delete data.uvs;
	delete data.normals;
}

void ModuleFBX::DrawMeshes()
{
	for (std::vector<ModelConfig>::iterator item = App->fbx->meshes.begin(); item != App->fbx->meshes.end(); ++item)
		App->renderer3D->DrawMeshes(*item);
}

bool ModuleFBX::CleanUp()
{
	aiDetachAllLogStreams();
	ClearMeshes();
	return true;

	return true;
}

bool ModuleFBX::LoadFBX(const char* path)
{
	
	LOG("Loading FBX...");
	file_name.clear();
	this->path = path;
	//Substract the name of the file
	std::string name(path);
	this->file_name = name.substr(name.find_last_of('\\') + 1);
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;
		for (int i = 0; i < rootNode->mNumChildren; ++i)
		{
			LoadModel(scene, rootNode->mChildren[i], path);
		}

		// ---- Release resources ----
		aiReleaseImport(scene);
		LOG("---- FBX LOADED WITH SUCCESS ----");
		return true;
	}
	else
		LOG("---- ERROR, COULDN'T LOAD FBX ----");
	return false;
}

void ModuleFBX::LoadModel(const aiScene* scene, aiNode* node, const char* path)
{

	if (node->mNumMeshes <= 0)
	{
		LOG("Unable to load the mesh with path: %s. The number of meshes is below or equal to 0.", path);
	}
	else
	{
		LOG("Loading mesh from path %s", path);
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[i]];
			mesh = ModelConfig();
			mesh.num_vertices = new_mesh->mNumVertices;
			mesh.vertices = new uint[mesh.num_vertices * 3];
			memcpy(mesh.vertices, new_mesh->mVertices, sizeof(float) * mesh.num_vertices * 3);

			// ---- Generate buffers ----
			glGenBuffers(1, (GLuint*) & (mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

			// ---- Geometry ----
			if (new_mesh->HasFaces())
			{
				mesh.num_indices = new_mesh->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices];
				for (uint j = 0; j < new_mesh->mNumFaces; j++)
				{
					if (new_mesh->mFaces[j].mNumIndices == 3)
					{
						memcpy(&mesh.indices[j * 3], new_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}

				glGenBuffers(1, (GLuint*) & (mesh.id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
			}
			else
			{
				LOG("Mesh with %i faces can not be loaded.", new_mesh->mNumFaces);
			}


		}
	}
}

uint ModuleFBX::CreateTextureID(const char* texture_path)
{
	uint texture_id;

	return texture_id;
}

void ModuleFBX::ApplyTexture(const char* path)
{
	
}

void const ModuleFBX::CentrateObjectView()
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)App->fbx->mesh.vertices, App->fbx->mesh.num_vertices);

	App->camera->Reference.x = box.CenterPoint().x;
	App->camera->Reference.y = box.CenterPoint().y;
	App->camera->Reference.z = box.CenterPoint().z;

	App->camera->Position.x = box.maxPoint.x * 2; // Increase the distance view
	App->camera->Position.y = box.maxPoint.y * 2;
	App->camera->Position.z = box.maxPoint.z * 2;

	App->camera->LookAt(App->camera->Reference);
}

math::AABB const ModuleFBX::GetAABB()
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)App->fbx->mesh.vertices, App->fbx->mesh.num_vertices);

	return box;
}

uint const ModuleFBX::MeshesSize()
{
	return meshes.size();
}

uint const ModuleFBX::GetIndices()
{
	return(mesh.num_indices);
}

uint const ModuleFBX::GetVertices()
{
	return(mesh.num_vertices);
}

vec3 const ModuleFBX::GetPosition()
{
	return(mesh.position);
}

vec3 const ModuleFBX::GetRotation()
{
	return(mesh.rotation);
}

vec3 const ModuleFBX::GetScale()
{
	return(mesh.scale);
}

float const ModuleFBX::GetNormals()
{
	return(mesh.num_normals);
}

float const ModuleFBX::GetUvs()
{
	return(mesh.num_uvs);
}

uint const ModuleFBX::GetTextureId()
{
	return(mesh.texture_id);
}