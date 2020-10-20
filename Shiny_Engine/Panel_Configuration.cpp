#include "Application.h"
#include "Globals.h"
#include "Module.h"
#include "Panel_Configuration.h"
#include "ModuleHardware.h"
#include "ModuleCamera3D.h"
#include "Light.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "MathGeoLib\MathGeoLib.h"
#include "mmgr\mmgr.h"



#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class Application* App;

PanelConfig::PanelConfig() : Panel("Config")
{
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	active = true;

}

PanelConfig::~PanelConfig() {}

int PanelConfig::GetFPS() {
	return fpsCap;
}


void PanelConfig::Draw()
{
	ImGui::Text("Configuration Menu");
	ImGui::Separator();
	ImGui::Separator();

	
	if (ImGui::CollapsingHeader("Application")) {
		ImGui::TextWrapped("App Name: Shiny Engine");
		ImGui::TextWrapped("Organization: UPC CITM");
		ImGui::Separator();
		ImGui::Separator();
	
		for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
			fps_array[i] = fps_array[i + 1];
		}
		fps_array[GRAPH_ARRAY_SIZE - 1] = ImGui::GetIO().Framerate;
		char fps_title[25];
		sprintf_s(fps_title, 25, "Framerate %.1f", fps_array[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", fps_array, IM_ARRAYSIZE(fps_array), 30, fps_title, 0.0f, 130.0f, ImVec2(0, 80));

		for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
			ms_array[i] = ms_array[i + 1];
		}
		ms_array[GRAPH_ARRAY_SIZE - 1] = 1000.0f / ImGui::GetIO().Framerate;
		char ms_title[25];
		sprintf_s(ms_title, 25, "Milliseconds %.1f", ms_array[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", ms_array, IM_ARRAYSIZE(ms_array), 30, ms_title, 0.0f, 130.0f, ImVec2(0, 80));

		sMStats smstats = m_getMemoryStatistics();

		for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
			mem_array[i] = mem_array[i + 1];
		}
		mem_array[GRAPH_ARRAY_SIZE - 1] = smstats.totalActualMemory;
		char mem_title[25] = "Memory Consumption";
	
		ImGui::PlotHistogram("", mem_array, IM_ARRAYSIZE(mem_title), 30, mem_title, 0.0f, 1000000.0f, ImVec2(0, 80));
	}

	
	if (ImGui::CollapsingHeader("Window")) {
		
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
	
		ImGui::SliderFloat("Brightness", &brightness, 0, 2, NULL);
	
		ImGui::SliderInt("Width", &width, 0, 1920, NULL);
		
		ImGui::SliderInt("Height", &height, 0, 1080, NULL);
		
	
		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
		{
			App->window->SetFullScreen(&App->window->fullscreen);
		}
		

		if (ImGui::Checkbox("Windowed", &App->window->windowed))
		{
			App->window->SetWindowed(&App->window->windowed);
		}

		if (ImGui::Checkbox("Full Desktop", &App->window->full_desktop))
		{
			App->window->SetWindowFullDesktop();
			width = 1920;
			height = 1080;
		}

	}

	if (ImGui::CollapsingHeader("Input")) {
		ImGui::Text("Mouse Positiom: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "X: %i ", App->input->GetMouseX());
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "Y: %i", App->input->GetMouseY());

		ImGui::Text("Mouse Motion: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "X: %i ", App->input->GetMouseXMotion());
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "Y: %i", App->input->GetMouseYMotion());

		ImGui::Text("Mouse Wheel: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "%i ", App->input->GetMouseZ());
	}
}