#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ModuleScene.h"
#include "parson/parson.h"

ComponentMesh::ComponentMesh(Application* app_parent, GameObject* parent) : Component(app_parent, parent, CompMesh)
{
	parent->components.push_back(this);
}


ComponentMesh::~ComponentMesh()
{
	App->renderer3D->mesh_list.remove(this);
	App->resources->ResourceUsageDecreased(mesh);
	gameObject->boundingBox.SetNegativeInfinity();
	gameObject->originalBoundingBox.SetNegativeInfinity();
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

void ComponentMesh::Draw()
{
	if (gameObject->active && print)
	{
		ComponentTransform* transform = gameObject->transform;
		glPushMatrix();
		float4x4 mat = transform->GetMatrixOGL();

		glMultMatrixf(mat.ptr());

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex.id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index.id);

	}
}

void ComponentMesh::Save(JSON_Object* parent)
{
	json_object_set_number(parent, "Type", type);
	json_object_set_number(parent, "UUID", uuid);
	json_object_set_string(parent, "Name", mesh->name.c_str());

}

void ComponentMesh::Load(JSON_Object* parent)
{
	uuid = json_object_get_number(parent, "UUID");

	std::string name = json_object_get_string(parent, "Name");

	
}