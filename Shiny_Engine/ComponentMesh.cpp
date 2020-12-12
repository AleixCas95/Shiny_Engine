#include "ComponentMesh.h"
#include "GameObject.h"
#include "Glew/include/glew.h"
#include "Application.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ModuleScene.h"
#include "ModuleFBX.h"

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
		ImGui::Checkbox("Mesh Active", &print);
		ImGui::Text("Number of vertices: %u", mesh->vertex.size);
		ImGui::Text("Number of faces: %u", mesh->index.size / 3);

		ImGui::Checkbox("Vertex normals", &printVertexNormals);

		ImGui::Text("Resource used %i times", mesh->usage);

		if (ImGui::Button("Delete Mesh"))
		{
			App->gobject->componentsToDelete.push_back(this);
		}
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

		if (gameObject)
		{
			ComponentTexture* tex = (ComponentTexture*)gameObject->GetComponent(CompTexture);
			if (tex)
			{
				if (tex->print)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, tex->GetID());
				}
			}
		}


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

	mesh = new ResourceMesh(name.c_str());

	App->renderer3D->mesh_list.push_back(this);

	App->resources->AddResource(mesh);

	
}