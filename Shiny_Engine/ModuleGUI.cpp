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

	//ImGui::ShowTestWindow();
	//static bool show_test = false;
	//static bool custom = false;
	//static bool normal = true;

	//GLint total_memory;
	//GLint memory_usage = 0;
	//GLint dedicated_memory = 0;
	//GLint available_memory = 0;

	//glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
	//glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_memory);
	//glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);

	//memory_usage = total_memory - available_memory;

	//if (show_test)
	//	ImGui::ShowTestWindow();
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
		ImGui::EndMainMenuBar();
	}
	if (ImGui::Begin("Configuration"))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
		
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
		}
	}
	ImGui::End();
	ImGui::Render();
	return UPDATE_CONTINUE;
}
