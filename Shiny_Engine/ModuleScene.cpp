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
	PlaneGrid p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	App->console->AddLog("Hit!");
}

