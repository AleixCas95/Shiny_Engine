#include "ComponentMesh.h"
#include "GameObject.h"



ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent, CompMesh)
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
