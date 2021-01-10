#ifndef MODULEFBX_H
#define MODULEFBX_H


#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include <vector>
#include "Glew\include\glew.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "ParShapes/par_shapes.h"

class ResourceMesh;
class ResourceTexture;

class aiScene;
class aiNode;
class aiMaterial;

struct ModelConfig
{
	//Geometry
	uint id_vertices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0;
	uint num_vertices = 0;
	uint* vertices = nullptr;

	//Texture
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;

	uint texture_id = 0;

	//Transformation
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

class ModuleFBX : public Module
{
public:
	ModuleFBX(Application* app, bool start_enabled = true);
	~ModuleFBX();

	// 

	bool Start();
	bool CleanUp();
	ResourceMesh* MeshParShape(par_shapes_mesh* mesh, const char* name, scriptType name_);

	bool LoadFBX(const char* path);
	GameObject* LoadMeshNode(const aiScene* scene, aiNode* node, GameObject* parent, const char* path, scriptType _name);

	void SaveMeshImporter(ResourceMesh* m, const uint& uuid, char* path = nullptr);
	void LoadMeshImporter(ResourceMesh* m, const uint& uuid, char* buff);
	void RealLoadTexture(const char* path, uint& texture_id);
	void ImportTexture(const char* path, scriptType name_);
	void ImportTextureGo(const char* path, GameObject* go, scriptType name_);
	bool LoadMesh(const char* exported_file, ResourceMesh* mesh);


	// Read only
	uint GetIndicesQuantity() const;
	uint GetVerticesQuantity() const;
	vec3 GetPosition() const;
	vec3 GetRotation() const;
	vec3 GetScale() const;
	float GetNormalsQuanity() const;
	float GetUvsQuantity() const;
	uint GetTextureId() const;
	void CentrateObjectView() const;
	math::AABB GetAABB() const;
	uint MeshesSize() const;




public:
	std::string path;
	std::string texture_path;
	int textureWidth = 0;
	int textureHeight = 0;
	std::string file_name;
	uint last_texture_id = 0;

	GLfloat fbx_position[3] = { 0.0f, 0.0f, 0.0f };

	uint checkerImageID = 0u;

	scriptType uuid;

private:
	ModelConfig data;
	ModelConfig mesh;
};

#endif // !MODULEFBX_H