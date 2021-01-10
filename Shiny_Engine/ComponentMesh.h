#pragma once
#include "Component.h"
#include "ResourcesMesh.h"
#include <string>

class ComponentMesh :
	public Component
{
public:
	ComponentMesh(Application* app_parent, GameObject* parent);
	~ComponentMesh();

	void Inspector();

	void Draw();

	void Save(JSON_Object* parent);

	void Load(JSON_Object* parent);

	bool IsPrimitive() const;
	void SetPrimitive(PrimitiveType type);

	void SetPath(const char* path);
	bool HasMesh() const;

public:
	ResourceMesh* mesh;

	scriptType uuid_mesh = 0;

	bool print = true;

	bool printVertexNormals = false;
	bool printFacesNormals = false;

	const char* path = nullptr;

	PrimitiveType is_primitive = PRIMITIVE_NONE;
};