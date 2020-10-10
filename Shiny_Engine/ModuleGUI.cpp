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


	GLint total_memory = 0;
	GLint memory_usage = 0;
	GLint dedicated_memory = 0;
	GLint available_memory = 0;


	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_memory);
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);
	memory_usage = total_memory - available_memory;


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
		/*	ImGui::InputText("App name", App->app_name, 20);
			ImGui::InputText("Organization", App->organization, 20);

			if (vector_fps.size() != 100)
			{
				vector_fps.push_back(App->GetFPS());
				vector_ms.push_back(App->GetMS());
			}

			else
			{
				vector_fps.erase(vector_fps.begin());
				vector_fps.push_back(App->GetFPS());

				vector_ms.erase(vector_ms.begin());
				vector_ms.push_back(App->GetMS());
			}

			char title[25];

			sprintf_s(title, 25, "Framerate %.1f", vector_fps[vector_fps.size() - 1]);
			ImGui::PlotHistogram("##framerate", &vector_fps[0], vector_fps.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

			sprintf_s(title, 25, "Milliseconds %.1f", vector_ms[vector_ms.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &vector_ms[0], vector_ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));*/
		}
		if (ImGui::CollapsingHeader("File System"))
		{
			ImGui::Text("Text: ");
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Text: ");
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", version.major, version.minor, version.patch);
			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i (Cache:%ikb)", CPU_Count, CPU_Cache);

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%iGb", ram);

			ImGui::Text("Caps: ");
			ImGui::SameLine();
			if (SDL_HasAVX)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX,");
			ImGui::SameLine();
			if (SDL_HasMMX)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MMX,");
			ImGui::SameLine();
			if (SDL_HasSSE)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE,");
			ImGui::SameLine();
			if (SDL_HasSSE2)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE2,");
			ImGui::SameLine();
			if (SDL_HasSSE3)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE3,");
			ImGui::SameLine();
			if (SDL_HasSSE41)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE41,");
			ImGui::SameLine();
			if (SDL_HasSSE42)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE42,");
			ImGui::SameLine();
			if (SDL_HasRDTSC)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RDTSC,");

			ImGui::Separator();
			ImGui::Text("GPU:  ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));
			ImGui::Text("Brand: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_RENDERER));
			ImGui::Text("VRAM Budget: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f Mb", (total_memory * 0.001));
			ImGui::Text("VRAM Usage: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f Mb", (memory_usage * 0.001));
			ImGui::Text("VRAM Available: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f Mb", (available_memory * 0.001));
			ImGui::Text("VRAM Reserved: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f Mb", (dedicated_memory * 0.001));
		}
		if (ImGui::CollapsingHeader("Options"))
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
			{
				int ll = SDL_SetWindowBrightness(App->window->window, App->window->brightness);
				SDL_UpdateWindowSurface(App->window->window);
			}

			if (ImGui::SliderInt("Width", &App->window->width, 1, 2000) || ImGui::SliderInt("Height", &App->window->height, 1, 2000))
			{
				/*SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
				SDL_UpdateWindowSurface(App->window->window);*/
			}

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			{
				if (App->window->fullscreen)
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
				else
					SDL_SetWindowFullscreen(App->window->window, App->window->flags);
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Resizable", &App->window->resizable))
			{
				if (App->window->resizable)
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
				else
					SDL_SetWindowFullscreen(App->window->window, App->window->flags);
			}

			if (ImGui::Checkbox("Bordeless ", &App->window->bordeless))
			{
				SDL_SetWindowBordered(App->window->window, (SDL_bool)!App->window->bordeless);
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Fullscreen Desktop", &App->window->fullscreen_desktop))
			{
				if (App->window->fullscreen_desktop)
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				else
					SDL_SetWindowFullscreen(App->window->window, App->window->flags);
			}
		}
	}
	ImGui::End();
	ImGui::Render();
	return UPDATE_CONTINUE;
}
