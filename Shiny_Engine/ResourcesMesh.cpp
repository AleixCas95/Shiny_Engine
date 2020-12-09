#include "ResourcesMesh.h"
#include "Glew/include/glew.h"

ResourceMesh::ResourceMesh(const char* path) : Resource(ResourceType::Mesh, path)
{
}

ResourceMesh::~ResourceMesh()
{
	glDeleteBuffers(1, (GLuint*) & (index.id));
	glDeleteBuffers(1, (GLuint*) & (vertex.id));
	glDeleteBuffers(1, (GLuint*) & (normals.id));
	glDeleteBuffers(1, (GLuint*) & (uvs.id));

	delete index.data;
	delete vertex.data;
	delete normals.data;
	delete uvs.data;
}

void ResourceMesh::Unload()
{
	/// TODO
}