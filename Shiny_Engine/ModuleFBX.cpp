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
#include "Resources.h"
#include "ResourcesTexture.h"
#include "ResourcesMesh.h"

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

	//LoadFBX("Game\\Assets\\Models\\BakerHouse.fbx");

	return ret;
}




bool ModuleFBX::CleanUp()
{
	aiDetachAllLogStreams();

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

GameObject* ModuleFBX::LoadMeshNode(const aiScene* scene, aiNode* node, GameObject* parent, const char* path)
{
	GameObject* go = new GameObject(App, parent, node->mName.C_Str());

	if (node->mNumMeshes > 0)
	{
		aiMesh* new_mesh = scene->mMeshes[node->mMeshes[0]];

		std::string _path = path;
		std::string _name = node->mName.C_Str();

		ResourceMesh* m = (ResourceMesh*)App->resources->GetResource(ResourceType::Mesh, (_path + _name).c_str());
		if (m == nullptr)
		{
			m = new ResourceMesh((_path + _name).c_str());
			m->vertex.size = new_mesh->mNumVertices * 3;
			m->vertex.data = new float[m->vertex.size];
			memset(m->vertex.data, 0, sizeof(float) * m->vertex.size);
			memcpy(m->vertex.data, new_mesh->mVertices, sizeof(float) * m->vertex.size);
			LOG("New mesh with %d vertices", m->vertex.size);

			//Load bounding box
			go->originalBoundingBox.Enclose((float3*)m->vertex.data, m->vertex.size / 3);
			go->boundingBox = go->originalBoundingBox;

			if (new_mesh->HasFaces())
			{
				m->index.size = new_mesh->mNumFaces * 3;
				m->index.data = new uint[m->index.size];
				memset(m->index.data, 0, sizeof(float) * m->index.size);
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
				m->uvs.size = new_mesh->mNumVertices * 2;
				m->uvs.data = new float[m->uvs.size];

				for (int i = 0; i < new_mesh->mNumVertices; ++i)
				{
					memcpy(&m->uvs.data[i * 2], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&m->uvs.data[(i * 2) + 1], &new_mesh->mTextureCoords[0][i].y, sizeof(float));
				}

				glGenBuffers(1, (GLuint*) & (m->uvs.id));
				glBindBuffer(GL_ARRAY_BUFFER, m->uvs.id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->uvs.size, m->uvs.data, GL_STATIC_DRAW);
			}

			if (new_mesh->HasNormals()) {
				m->hasNormals = true;
				m->normals.size = new_mesh->mNumVertices * 3;
				m->normals.data = new float[m->normals.size];
				memcpy(m->normals.data, new_mesh->mNormals, sizeof(float) * m->normals.size);
			}
			glGenBuffers(1, (GLuint*) & (m->vertex.id));
			glBindBuffer(GL_ARRAY_BUFFER, m->vertex.id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->vertex.size, m->vertex.data, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*) & (m->index.id));
			glBindBuffer(GL_ARRAY_BUFFER, m->index.id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->index.size, m->index.data, GL_STATIC_DRAW);

			App->resources->AddResource(m);
		}
		else
		{
			App->resources->ResourceUsageIncreased(m);
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

				textPath = "Assets\\Textures\\" + textPath;

				ImportTextureGo(textPath.c_str(), go);
			}
		}

		ComponentMesh* newMesh = new ComponentMesh(App, go);
		newMesh->mesh = m;

		SaveMeshImporter(m, newMesh->uuid);

		App->renderer3D->mesh_list.push_back(newMesh);

		

		LOG("Mesh loaded");
	}

	float4x4 transformMat(
		node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
		node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
		node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
		node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);

	go->transform->SetTransform(transformMat);

	for (int child = 0; child < node->mNumChildren; ++child)
	{
		LoadMeshNode(scene, node->mChildren[child], go, path);
	}
	return go;
}

void ModuleFBX::SaveMeshImporter(ResourceMesh* m, const uint& uuid, char* path)
{
	uint ranges[4] = { m->index.size, m->vertex.size, m->normals.size, m->uvs.size };
	float size =
		sizeof(ranges) +
		sizeof(uint) * m->index.size +
		sizeof(float) * m->vertex.size +
		sizeof(float) * m->normals.size +
		sizeof(float) * m->uvs.size;

	char* meshBuffer = new char[size];
	char* cursor = meshBuffer;

	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	cursor += bytes;
	bytes = sizeof(uint) * m->index.size;
	memcpy(cursor, m->index.data, bytes);

	cursor += bytes;
	bytes = sizeof(float) * m->vertex.size;
	memcpy(cursor, m->vertex.data, bytes);

	if (m->normals.data)
	{
		cursor += bytes;
		bytes = sizeof(float) * m->normals.size;
		memcpy(cursor, m->normals.data, bytes);
	}

	if (m->uvs.data)
	{
		cursor += bytes;
		bytes = sizeof(float) * m->uvs.size;
		memcpy(cursor, m->uvs.data, bytes);
	}

	App->resources->SaveFile(size, meshBuffer, ResourceType::Mesh, uuid, path);

	delete[] meshBuffer;
}
void ModuleFBX::LoadMeshImporter(ResourceMesh* m, const uint& uuid, char* buff)
{
	uint ranges[4];

	char* cursor = buff;

	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	m->index.size = ranges[0];
	m->vertex.size = ranges[1];
	m->normals.size = ranges[2];
	m->uvs.size = ranges[3];

	cursor += bytes;
	bytes = sizeof(uint) * m->index.size;
	m->index.data = new uint[m->index.size];
	memcpy(m->index.data, cursor, bytes);

	glGenBuffers(1, (GLuint*) & (m->index.id));
	glBindBuffer(GL_ARRAY_BUFFER, m->index.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->index.size, m->index.data, GL_STATIC_DRAW);

	cursor += bytes;
	bytes = sizeof(float) * m->vertex.size;
	m->vertex.data = new float[m->vertex.size];
	memcpy(m->vertex.data, cursor, bytes);

	glGenBuffers(1, (GLuint*) & (m->vertex.id));
	glBindBuffer(GL_ARRAY_BUFFER, m->vertex.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->vertex.size, m->vertex.data, GL_STATIC_DRAW);

	cursor += bytes;
	bytes = sizeof(float) * m->normals.size;
	m->normals.data = new float[m->normals.size];
	memcpy(m->normals.data, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(float) * m->uvs.size;
	m->uvs.data = new float[m->uvs.size];
	memcpy(m->uvs.data, cursor, bytes);

	glGenBuffers(1, (GLuint*) & (m->uvs.id));
	glBindBuffer(GL_ARRAY_BUFFER, m->uvs.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->uvs.size, m->uvs.data, GL_STATIC_DRAW);

	delete buff;
}

void ModuleFBX::RealLoadTexture(const char* path, uint& texture_id)
{
	ilInit();
	iluInit();
	ilutInit();
	if (ilLoadImage(path))
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint size;
		ILubyte* data;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		size = ilSaveL(IL_DDS, NULL, 0);
		if (size > 0) {
			data = new ILubyte[size];
			if (ilSaveL(IL_DDS, data, size) > 0)
				App->resources->SaveFile(size, (char*)data, ResourceType::Texture, 0u, path);
			delete[] data;
		}

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
	}
	else
	{
		LOG("Couldn't load texture");
	}
}

void ModuleFBX::ImportTexture(const char* path)
{
	ResourceTexture* m = (ResourceTexture*)App->resources->GetResource(ResourceType::Texture, path);
	if (m == nullptr)
	{
		m = new ResourceTexture(path);
		uint texture_id = 0;

		RealLoadTexture(path, texture_id);
		m->id = texture_id;

		App->resources->AddResource(m);
	}
	else
	{
		App->resources->ResourceUsageIncreased(m);
	}

	if (App->scene->current_object->HasComponent(CompTexture))
	{
		ComponentTexture* texture = (ComponentTexture*)App->scene->current_object->GetComponent(CompTexture);
		App->resources->ResourceUsageDecreased(texture->RTexture);
		std::string tex_path(path);
		texture->path = tex_path;
		texture->RTexture = m;
		LOG("Texture loaded");
	}
	else
	{
		ComponentTexture* texture = new ComponentTexture(App, App->scene->current_object);
		std::string tex_path(path);
		texture->path = tex_path;
		texture->RTexture = m;
		LOG("Texture loaded");
	}
}

void ModuleFBX::ImportTextureGo(const char* path, GameObject* go)
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
			ComponentTexture* texture = new ComponentTexture(App, go);
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


Mesh* ModuleFBX::MeshParShape(par_shapes_mesh* mesh, const char* name)
{
	GameObject* go = new GameObject(App, App->gobject->root, name);

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

	glGenBuffers(1, (GLuint*) & (m->vertex.id));
	glBindBuffer(GL_ARRAY_BUFFER, m->vertex.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->vertex.size, m->vertex.data, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) & (m->index.id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->index.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * m->index.size, m->index.data, GL_STATIC_DRAW);

	ComponentMesh* newMesh = new ComponentMesh(App, go);
	newMesh->mesh = m;

	App->renderer3D->mesh_list.push_back(m);

	App->console->AddLog("Par_Shapes Mesh loaded");

	return m;
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
