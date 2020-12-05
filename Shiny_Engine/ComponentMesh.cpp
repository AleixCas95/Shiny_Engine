#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"
#include "parson/parson.h"

ComponentMesh::ComponentMesh(Application* papaito, GameObject* parent) : Component(papaito, parent, CompMesh)
{
	parent->components.push_back(this);
}


ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Inspector()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Number of vertices: %u", mesh->vertex.size);
		ImGui::Text("Number of faces: %u", mesh->index.size / 3);

		ImGui::Checkbox("Vertex normals", &printVertexNormals);
	}
}

void ComponentMesh::Save(JSON_Object* parent)
{
	json_object_set_number(parent, "Type", type);
	json_object_set_number(parent, "UUID", uuid);
	
}

void ComponentMesh::Load(JSON_Object* parent)
{
	
}