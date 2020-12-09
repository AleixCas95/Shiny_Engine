#pragma once
#include "Resources.h"


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
	ResourceMesh(const char* path);
	~ResourceMesh();

	void Unload();

public:

	int id = -1;
	buffer<unsigned int> index;
	buffer<float> vertex;
	buffer<float> normals;
	buffer<float> uvs;

	bool hasNormals = false;
};

