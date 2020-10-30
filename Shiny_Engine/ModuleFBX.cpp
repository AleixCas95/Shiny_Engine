#include "Application.h"
#include "ModuleFBX.h"
#include "ModuleRenderer3D.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Devil\include\il.h"
#include "Devil\include\ilut.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "ParShapes/par_shapes.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleConsole.h"

#pragma comment (lib, "Assimp\\libx86\\assimp.lib")
#pragma comment (lib, "Devil\\libx86\\DevIL.lib")
#pragma comment ( lib, "Devil\\libx86\\ILU.lib" )
#pragma comment ( lib, "Devil\\libx86\\ILUT.lib" )

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
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);

	LoadFBX("Game\\Assets\\Models\\BakerHouse.fbx");

	return ret;
}

void ModuleFBX::ClearMeshes()
{
	for (int i = App->renderer3D->meshes.size() - 1; App->renderer3D->meshes.size() != 0; i--)
	{
		delete[] App->renderer3D->meshes[i].indices;
		delete[] App->renderer3D->meshes[i].vertices;
		delete[] App->renderer3D->meshes[i].uvs;
		delete[] App->renderer3D->meshes[i].normals;
		App->renderer3D->meshes.pop_back();
	}

	//Geometry
	delete data.indices;
	delete data.vertices;

	//Texture
	delete data.uvs;
	delete data.normals;
}

bool ModuleFBX::CleanUp()
{
	aiDetachAllLogStreams();
	ClearMeshes();
	path.clear();
	texture_path.clear();
	return true;
}

bool ModuleFBX::LoadFBX(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* newGameObject = App->scene->current_object = LoadMeshNode(scene, scene->mRootNode, App->gobject->root);

		aiReleaseImport(scene);
	}
	else
		App->console->AddLog("Error loading scene %s", path);
	return false;
}

GameObject* ModuleFBX::LoadMeshNode(const aiScene* scene, aiNode* node, GameObject* parent)
{
	GameObject* go = new GameObject(parent, node->mName.C_Str());

	if (node->mNumMeshes > 0)
	{
		aiMesh* new_mesh = scene->mMeshes[node->mMeshes[0]];

		Mesh* m = new Mesh(go);

		m->vertex.size = new_mesh->mNumVertices;
		m->vertex.data = new float[m->vertex.size * 3];
		memcpy(m->vertex.data, new_mesh->mVertices, sizeof(float) * m->vertex.size * 3);
		LOG("New mesh with %d vertices", m->vertex.size);

		if (new_mesh->HasFaces())
		{
			m->index.size = new_mesh->mNumFaces * 3;
			m->index.data = new uint[m->index.size];
			for (uint i = 0; i < new_mesh->mNumFaces; ++i)
			{
				if (new_mesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}

				else
				{
					memcpy(&m->index.data[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}
		}
		if (new_mesh->HasTextureCoords(m->uvs.id))
		{
			m->uvs.size = new_mesh->mNumVertices;
			m->uvs.data = new float[m->uvs.size * 2];

			for (int i = 0; i < new_mesh->mNumVertices; ++i)
			{
				memcpy(&m->uvs.data[i * 2], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
				memcpy(&m->uvs.data[(i * 2) + 1], &new_mesh->mTextureCoords[0][i].y, sizeof(float));
			}

			glGenBuffers(1, (GLuint*) & (m->uvs.id));
			glBindBuffer(GL_ARRAY_BUFFER, m->uvs.id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m->uvs.size, m->uvs.data, GL_STATIC_DRAW);
		}

		if (scene->HasMaterials())
		{
			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];

			if (material)
			{
				aiString textName;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textName);

				std::string textPath(textName.data);

				textPath = textPath.substr(textPath.find_last_of("\\") + 1);

				textPath = "Game\\Assets\\Textures\\" + textPath;

				ImportTexture(textPath.c_str(), go);
			}
		}

		if (new_mesh->HasNormals()) {
			m->hasNormals = true;
			m->normals.size = new_mesh->mNumVertices;
			m->normals.data = new float[m->normals.size * 3];
			memcpy(m->normals.data, new_mesh->mNormals, sizeof(float) * m->normals.size * 3);
		}
		glGenBuffers(1, (GLuint*) & (m->vertex.id));
		glBindBuffer(GL_ARRAY_BUFFER, m->vertex.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->vertex.size, m->vertex.data, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*) & (m->index.id));
		glBindBuffer(GL_ARRAY_BUFFER, m->index.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->index.size, m->index.data, GL_STATIC_DRAW);

		ComponentMesh* newMesh = new ComponentMesh(go);
		newMesh->mesh = m;

		App->renderer3D->mesh_list.push_back(m);
		LOG("Mesh loaded");
	}
	for (int child = 0; child < node->mNumChildren; ++child)
	{
		LoadMeshNode(scene, node->mChildren[child], go);
	}
	return go;
}

void ModuleFBX::ImportTexture(const char* path)
{
	ilInit();
	iluInit();
	ilutInit();
	if (ilLoadImage(path))
	{
		uint texture_id = 0;

		uint id = 0;

		ilGenImages(1, &id);
		ilBindImage(id);
		ilLoadImage(path);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		texture_id = ilutGLBindTexImage();
		glBindTexture(GL_TEXTURE_2D, 0);
		ilDeleteImages(1, &id);

		if (App->scene->current_object->HasComponent(CompTexture))
		{
			ComponentTexture* texture = (ComponentTexture*)App->scene->current_object->GetComponent(CompTexture);
			glDeleteTextures(1, &texture->tex_id);
			texture->tex_id = texture_id;
			std::string tex_path(path);
			texture->path = tex_path;
			LOG("Texture loaded");
		}
		else
		{
			ComponentTexture* texture = new ComponentTexture(App->scene->current_object);
			texture->tex_id = texture_id;
			std::string tex_path(path);
			texture->path = tex_path;
			LOG("Texture loaded");
		}
	}
	else
	{
		LOG("Couldn't load texture");
	}
}

void ModuleFBX::ImportTexture(const char* path, GameObject* go)
{
	ilInit();
	iluInit();
	ilutInit();
	if (ilLoadImage(path))
	{
		uint texture_id = 0;

		uint id = 0;

		ilGenImages(1, &id);
		ilBindImage(id);
		ilLoadImage(path);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		texture_id = ilutGLBindTexImage();
		glBindTexture(GL_TEXTURE_2D, 0);
		ilDeleteImages(1, &id);


		if (go->HasComponent(CompTexture))
		{
			ComponentTexture* texture = (ComponentTexture*)go->GetComponent(CompTexture);
			glDeleteTextures(1, &texture->tex_id);
			texture->tex_id = texture_id;
			std::string tex_path(path);
			texture->path = tex_path;
			LOG("Texture loaded");
		}
		else
		{
			ComponentTexture* texture = new ComponentTexture(go);
			texture->tex_id = texture_id;
			std::string tex_path(path);
			texture->path = tex_path;
			LOG("Texture loaded");
		}
	}
	else
	{
		LOG("Couldn't load texture: %s", path);
	}
}

void ModuleFBX::CentrateObjectView()const
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

math::AABB ModuleFBX::GetAABB()const
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)App->fbx->mesh.vertices, App->fbx->mesh.num_vertices);

	return box;
}

uint ModuleFBX::MeshesSize()const
{
	return App->renderer3D->meshes.size();
}

uint ModuleFBX::GetIndicesQuantity()const
{
	return(mesh.num_indices);
}

uint ModuleFBX::GetVerticesQuantity()const
{
	return(mesh.num_vertices);
}

vec3 ModuleFBX::GetPosition()const
{
	return(mesh.position);
}

vec3 ModuleFBX::GetRotation()const
{
	return(mesh.rotation);
}

vec3 ModuleFBX::GetScale()const
{
	return(mesh.scale);
}

float ModuleFBX::GetNormalsQuanity()const
{
	return(mesh.num_normals);
}

float ModuleFBX::GetUvsQuantity()const
{
	return(mesh.num_uvs);
}

uint ModuleFBX::GetTextureId()const
{
	return(mesh.texture_id);
}

Mesh* ModuleFBX::MeshParShape(par_shapes_mesh* mesh, const char* name)
{
	GameObject* go = new GameObject(App->gobject->root, name);

	Mesh* m = new Mesh(go);

	m->vertex.size = mesh->npoints;
	m->vertex.data = new float[m->vertex.size * 3];
	memcpy(m->vertex.data, mesh->points, sizeof(float) * mesh->npoints * 3);

	m->index.size = mesh->ntriangles * 3;
	m->index.data = new uint[m->index.size];

	for (int i = 0; i < m->index.size; i++)
	{
		m->index.data[i] = (uint)mesh->triangles[i];
	}

	glGenBuffers(1, (GLuint*)&(m->vertex.id));
	glBindBuffer(GL_ARRAY_BUFFER, m->vertex.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->vertex.size, m->vertex.data, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(m->index.id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->index.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * m->index.size, m->index.data, GL_STATIC_DRAW);

	ComponentMesh* newMesh = new ComponentMesh(go);
	newMesh->mesh = m;

	App->renderer3D->mesh_list.push_back(m);

	App->console->AddLog("Par_Shapes Mesh loaded");

	return m;
}