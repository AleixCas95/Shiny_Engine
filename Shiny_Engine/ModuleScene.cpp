#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "GameObject.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Glew\include\glew.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Globals.h"
#include "Resources.h"
#include "ModuleConsole.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

class GameObject;

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{

	App->console->AddLog("Loading Intro assets");
	bool ret = true;
	glewInit();
	ImGuizmo::Enable(true);
	App->camera->LookAt(math::float3(0, 0, 0));

	return ret;
}
update_status ModuleScene::PreUpdate(float dt)
{

	//ImGui_ImplSdlGL3_NewFrame(App->window->window);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	return(UPDATE_CONTINUE);
}

// Load assets
bool ModuleScene::CleanUp()
{
	App->console->AddLog("Unloading Intro scene");
	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		guiz_operation = ImGuizmo::BOUNDS;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		guiz_operation = ImGuizmo::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		guiz_operation = ImGuizmo::SCALE;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		guiz_operation = ImGuizmo::ROTATE;
	}

	if (current_object)
	{
		float4x4 transformGlobal = current_object->transform->GetMatrix();
		transformGlobal.Transpose();

		ImGuiIO& io = ImGui::GetIO();

		ImGuizmo::SetRect(0.f, 0.f, io.DisplaySize.x, io.DisplaySize.y);

		ImGuizmo::Manipulate(App->renderer3D->current_cam->GetViewMatrix().ptr(), App->renderer3D->current_cam->GetProjectionMatrix().ptr(), guiz_operation, guiz_mode, transformGlobal.ptr(), nullptr, nullptr);

		if (ImGuizmo::IsUsing() && Time::gameState == GameState::EDITOR)
		{
			transformGlobal.Transpose();
			if (current_object->parent)
			{
				float4x4 matrix = current_object->parent->transform->GetMatrix();
				matrix.Inverse();
				transformGlobal = matrix.Mul(transformGlobal);
			}
			float3 pos, scale;
			Quat rot;
			transformGlobal.Decompose(pos, rot, scale);
			switch (guiz_operation)
			{
			case ImGuizmo::TRANSLATE:
				current_object->transform->SetPos(pos);
				break;
			case ImGuizmo::ROTATE:
				current_object->transform->SetRotation(rot);
				break;
			case ImGuizmo::SCALE:
				current_object->transform->SetScale(scale);
				break;
			default:
				break;
			}
		}
	}

	PrimitivePlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent)
{
	GameObject* game_object = new GameObject(App, parent);

	return game_object;
}

GameObject* ModuleScene::GetRootGameObject() const
{
	return root_object;
}

GameObject* ModuleScene::GetFocusedGameObject() const {

	return root_object;
}


void ModuleScene::DestroyGOs() {
	RELEASE(root_object);
}

void ModuleScene::CreateRootObject() {
	root_object = CreateGameObject(nullptr);
	root_object->SetName("root");

}

GameObject* ModuleScene::GetGameObjectFromUUID(uint UUID, GameObject* root) const
{

	GameObject* ret = nullptr;


	if (root) {

		if (root->uuid == UUID)
			return root;

		else
		{
			if (root->GetNumChilds() > 0) {

				for (std::vector<GameObject*>::const_iterator it = root->childs.begin(); it < root->childs.end(); it++) {

					if (ret != nullptr)
						return ret;


					if ((*it)->uuid == UUID) {

						return *it;

					}
					else {

						ret = GetGameObjectFromUUID(UUID, *it);
					}

				}


			}
		}
	}
	return ret;
}
void ModuleScene::AssignMeshToGameObject(scriptType mesh_uuid)
{
	//If there isn't a mesh, create it
	if (current_object && !current_object->GetComponent(CompMesh))
		current_object->CreateComponent(CompMesh);

	if (current_object) {

		ComponentMesh* mesh = (ComponentMesh*)current_object->GetComponent(CompMesh);

		if (!mesh->IsPrimitive()) {
			//Unload previous mesh resource
			ResourceMesh* res_mesh;
			res_mesh = (ResourceMesh*)App->resources->Get(mesh->uuid_mesh);
			if (res_mesh)
				res_mesh->UnloadToMemory();

			//Load new mesh resource
			res_mesh = (ResourceMesh*)App->resources->Get(mesh_uuid);
			if (res_mesh)
				res_mesh->LoadToMemory();

			mesh->uuid_mesh = mesh_uuid;
			mesh->SetPath(res_mesh->GetFile());
			current_object->UpdateBoundingBox();
		}
		else
			App->console->AddLog("You cannot change the mesh of a primitive.");
	}
}

void ModuleScene::AssignTexToGameObject(scriptType tex_uuid)
{
	ResourceTexture* res_tex;
	if (current_object) {

		//Load new resource
		res_tex = (ResourceTexture*)App->resources->Get(tex_uuid);
		if (res_tex) {
			res_tex->LoadToMemory();
		}
		//App->gui->AddLogToConsole("Put a texture from the assets folder");
	}
	//else
		//App->gui->AddLogToConsole("Select a GameObject to assign a texture material to it.");
}

GameObject* ModuleScene::CreateGameObjectByMesh(UID mesh_uuid)
{
	//Load resource mesh
	ResourceMesh* res_mesh;
	res_mesh = (ResourceMesh*)App->resources->Get(mesh_uuid);
	if (res_mesh)
		res_mesh->LoadToMemory();

	//Create GameObject
	GameObject* go = App->scene->CreateGameObject(App->scene->GetRootGameObject());
	go->SetName(std::to_string(mesh_uuid).c_str()); //TODO: put name of the Assimp node name
	ComponentMesh* mesh = (ComponentMesh*)go->CreateComponent(CompMesh);
	mesh->uuid_mesh = mesh_uuid;
	mesh->SetPath(res_mesh->GetFile());
	go->UpdateBoundingBox();

	return go;
}