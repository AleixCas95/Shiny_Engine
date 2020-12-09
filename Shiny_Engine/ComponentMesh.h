#pragma once
#include "Component.h"
#include "Primitive.h"
#include <string>

class ComponentMesh :
	public Component
{
public:
	ComponentMesh(Application* app_parent, GameObject* parent);
	~ComponentMesh();

	void Inspector();


	void Save(JSON_Object* parent);

	void Load(JSON_Object* parent);
	

public:
	Mesh* mesh;
	std::string name;

	bool print = true;

	bool printVertexNormals = false;
	bool printFacesNormals = false;
};