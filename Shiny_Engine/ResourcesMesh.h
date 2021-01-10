#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__


#include "Resources.h"
#include "Component.h"
#include "ParShapes/par_shapes.h"

class GameObject;

template <typename T>
struct buffer
{
	uint id = 0u;
	uint size = 0u;
	T* data = nullptr;
};

class ResourceMesh : public Resource
{
public:
	ResourceMesh(scriptType uuid);
	~ResourceMesh();

	bool LoadInMemory();
	bool UnloadInMemory();

	void LoadMeshPrimitive(par_shapes_mesh* shape);

	void BindBuffers();
	void BindBuffersPrimitive();

	bool IsPrimitive() const;

public:
	PrimitiveType is_primitive = PRIMITIVE_NONE;

	//Vertex info
	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex_ = nullptr;

	//Index info
	uint id_index = 0;
	uint num_index = 0;
	uint* index_ = nullptr;

	//Normals info
	uint id_normal = 0;
	uint num_normal = 0;
	float* normal = nullptr;

	//UVs info
	uint id_uv = 0;
	uint num_uv = 0;
	float* uv = nullptr;

	int id = -1;
	buffer<unsigned int> index;
	buffer<float> vertex;
	buffer<float> normals;
	buffer<float> uvs;


	bool hasNormals = false;

};

#endif