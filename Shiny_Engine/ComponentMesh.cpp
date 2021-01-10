#include "ComponentMesh.h"
#include "GameObject.h"
#include "Glew/include/glew.h"
#include "Application.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ModuleScene.h"
#include "ModuleFBX.h"
#include "ResourcesMesh.h"



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
			App->renderer3D->mesh_list.remove(this);
		}

		if (ImGui::Button("Delete Object"))
		{
			/*App->renderer3D->mesh_list.remove(this);
			if (App->scene->current_object->parent)
			{
				App->scene->current_object->parent->childs(App->scene->current_object);

			}
			App->inspector->NewObjectsToDelete(App->scene->current_object);*/

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

			if (printVertexNormals && mesh->hasNormals)
			{
				int size = 2;
				glColor3f(0.0f, 1.0f, 0.0f);

				for (uint i = 0; i < mesh->vertex.size; i += 3)
				{
					glBegin(GL_LINES);
					glVertex3f(mesh->vertex.data[i], mesh->vertex.data[i + 1], mesh->vertex.data[i + 2]);
					glVertex3f(mesh->vertex.data[i] + mesh->normals.data[i] * size, mesh->vertex.data[i + 1] + mesh->normals.data[i + 1] * size, mesh->vertex.data[i + 2] + mesh->normals.data[i + 2] * size);
					glEnd();
				}
				glColor3f(1.0f, 1.0f, 1.0f);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, mesh->uvs.id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, mesh->index.size, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
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
	int aux = json_object_get_number(parent, "is primitive");
	is_primitive = (PrimitiveType)aux;
	uuid_mesh = json_object_get_number(parent, "UUID Mesh");
	active = json_object_get_boolean(parent, "Active");
	const char* p = json_object_get_string(parent, "path");
	char* ap = (char*)p;
	std::string fullpath;
	std::string file;
	App->files->SplitFilePath(p, &fullpath, &file);
	switch (is_primitive) {
	case PRIMITIVE_NONE:
		if (HasMesh()) {
			ResourceMesh* res = (ResourceMesh*)App->resources->Get(uuid_mesh);
			if (res)
				res->LoadToMemory();
#ifndef GAME_MODE
			else {
				string log = "GameObject " + (string)gameObject->GetName() + ": Charging a mesh component with no mesh";
				//	App->gui->AddLogToConsole(log.c_str());
			}
#endif
		}

		break;

	case PRIMITIVE_CUBE:
	case PRIMITIVE_SPHERE:
	case PRIMITIVE_PLANE:
	{
		ResourceMesh* res = App->resources->GetPrimitive(is_primitive);
		if (res)
			res->LoadToMemory();
		uuid_mesh = res->GetUID();
	}
	break;
	}

	gameObject->UpdateBoundingBox();
}

bool ComponentMesh::IsPrimitive() const
{
	if (is_primitive == PRIMITIVE_NONE)
		return false;

	return true;
}

void ComponentMesh::SetPrimitive(PrimitiveType type)
{
	is_primitive = type;
}

void ComponentMesh::SetPath(const char* path)
{
	this->path = path;
}

bool ComponentMesh::HasMesh() const
{
	return uuid_mesh != 0;
}
