#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )



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
	//App->camera->Move(vec3(1,1,0));
	App->camera->LookAt(math::float3(0,0,0));

	return ret;
}
update_status ModuleScene::PreUpdate(float dt)
{
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


	PlaneGrid p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	App->console->AddLog("Hit!");
}

