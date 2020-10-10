#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )


ModuleGUI ::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{}

bool ModuleGUI::Start()
{
	bool ret = true;
	ImGui_ImplSdlGL3_Init(App->window->window);
	glewInit();
	SDL_VERSION(&version);
	CPU_Cache = SDL_GetCPUCacheLineSize();
	CPU_Count = SDL_GetCPUCount();
	ram = SDL_GetSystemRAM();
	ram /= 1000;

	return ret;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
	return(UPDATE_CONTINUE);
}


bool ModuleGUI::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

update_status ModuleGUI::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O"))
			if (ImGui::MenuItem("Save", "Ctrl+S")) 
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
		}

		if (ImGui::BeginMenu("Help"))
		{
		}
		ImGui::EndMainMenuBar();
	}
	if (ImGui::Begin("Configuration"))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
		}
		if (ImGui::CollapsingHeader("File System"))
		{
		}
		if (ImGui::CollapsingHeader("Input"))
		{
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
		}
		if (ImGui::CollapsingHeader("Options"))
		{
		}
	}
	ImGui::End();
	ImGui::Render();
	return UPDATE_CONTINUE;
}
