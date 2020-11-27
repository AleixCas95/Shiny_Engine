#pragma once
#include "Component.h"
#include "Primitive.h"
#include <string>

class ComponentMesh :
	public Component
{
public:
	ComponentMesh(Application* papaito, GameObject* parent);
	~ComponentMesh();

	void Inspector();



public:
	Mesh* mesh;
	std::string name;

	bool print = true;

	bool printVertexNormals = false;
	bool printFacesNormals = false;
};