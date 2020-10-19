#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "mmgr\mmgr.h"
#include "Glew\include\glew.h"


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{}

bool ModuleGUI::Start()
{
	
	SDL_VERSION(&version);	
	ImGui_ImplSdlGL3_Init(App->window->window);
	glewInit();
	CPU_Cache = SDL_GetCPUCacheLineSize();
	CPU_Count = SDL_GetCPUCount();
	ram = SDL_GetSystemRAM();
	ram /= 1000;

	return true;
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
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", "Ctrl+A"))
			{
				

			

			
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (ImGui::Begin("Configuration"))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::InputText("App name", "Shine Engine", 50);
			ImGui::InputText("Organization", "Developed by Students at CITM", 50);

			//FPS Graph
			for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
			{
				fps_array[i] = fps_array[i + 1];
			}
			fps_array[GRAPH_ARRAY_SIZE - 1] = ImGui::GetIO().Framerate;
			char fps_title[25];
			sprintf_s(fps_title, 25, "Framerate %.1f", fps_array[GRAPH_ARRAY_SIZE - 1]);
			ImGui::PlotHistogram("", fps_array, IM_ARRAYSIZE(fps_array), 30, fps_title, 0.0f, 130.0f, ImVec2(0, 80));

			//MS Graph
			for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
			{
				ms_array[i] = ms_array[i + 1];
			}
			ms_array[GRAPH_ARRAY_SIZE - 1] = 1000.0f / ImGui::GetIO().Framerate;
			char ms_title[25];
			sprintf_s(ms_title, 25, "Milliseconds %.1f", ms_array[GRAPH_ARRAY_SIZE - 1]);
			ImGui::PlotHistogram("", ms_array, IM_ARRAYSIZE(ms_array), 30, ms_title, 0.0f, 130.0f, ImVec2(0, 80));

			//sM Stats
			sMStats smstats = m_getMemoryStatistics();

			//Memory Graph
			for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
			{
				mem_array[i] = mem_array[i + 1];
			}
			mem_array[GRAPH_ARRAY_SIZE - 1] = smstats.totalActualMemory;
			char mem_title[25] = "Memory Consumption";
			ImGui::PlotHistogram("", mem_array, IM_ARRAYSIZE(mem_title), 30, mem_title, 0.0f, 1000000.0f, ImVec2(0, 80));

	
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
				SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
				SDL_UpdateWindowSurface(App->window->window);
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

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();
			ImGui::TextColored(yellow_color, "%d.%d.%d", version.major, version.minor, version.patch);
			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(yellow_color, "%i (Cache:%ikb)", CPU_Count, CPU_Cache);

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(yellow_color, "%iGb", ram);

			ImGui::Text("Caps: ");
			ImGui::SameLine();
			if (SDL_HasAVX)
				ImGui::TextColored(yellow_color, "AVX,");
			ImGui::SameLine();
			if (SDL_HasMMX)
				ImGui::TextColored(yellow_color, "MMX,");
			ImGui::SameLine();
			if (SDL_HasSSE)
				ImGui::TextColored(yellow_color, "SSE,");
			ImGui::SameLine();
			if (SDL_HasSSE2)
				ImGui::TextColored(yellow_color, "SSE2,");
			ImGui::SameLine();
			if (SDL_HasSSE3)
				ImGui::TextColored(yellow_color, "SSE3,");
			ImGui::SameLine();
			if (SDL_HasSSE41)
				ImGui::TextColored(yellow_color, "SSE41,");
			ImGui::SameLine();
			if (SDL_HasSSE42)
				ImGui::TextColored(yellow_color, "SSE42,");
			ImGui::SameLine();
			if (SDL_HasRDTSC)
				ImGui::TextColored(yellow_color, "RDTSC,");

			ImGui::Separator();
			
			ImGui::Text("GPU:  "); ImGui::SameLine(); ImGui::TextColored(yellow_color, "%s", glGetString(GL_VENDOR));
			ImGui::Text("Brand: "); ImGui::SameLine(); ImGui::TextColored(yellow_color, "%s", glGetString(GL_RENDERER));
			ImGui::Text("VRAM Budget: ");	ImGui::SameLine(); ImGui::TextColored(yellow_color, "%.1f Mb", (total_memory * 0.001));
			ImGui::Text("VRAM Usage: "); ImGui::SameLine(); ImGui::TextColored(yellow_color, "%.1f Mb", (memory_usage * 0.001));
			ImGui::Text("VRAM Available: "); ImGui::SameLine(); ImGui::TextColored(yellow_color, "%.1f Mb", (available_memory * 0.001));
			ImGui::Text("VRAM Reserved: "); ImGui::SameLine(); ImGui::TextColored(yellow_color, "%.1f Mb", (dedicated_memory * 0.001));
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position: ");
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "X: %i ", App->input->GetMouseX());
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "Y: %i", App->input->GetMouseY());

			ImGui::Text("Mouse Motion: ");
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "X: %i ", App->input->GetMouseXMotion());
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "Y: %i", App->input->GetMouseYMotion());

			ImGui::Text("Mouse Wheel: ");
			ImGui::SameLine();
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, "%i ", App->input->GetMouseZ());
		}
	}
	ImGui::End();
	ImGui::Render();

	return UPDATE_CONTINUE;
}
