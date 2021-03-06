#include "Application.h"
#include "Globals.h"
#include "ModuleConfiguration.h"
#include "ModuleHardware.h"
#include "ModuleCamera3D.h"
#include "Light.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include "mmgr\mmgr.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

ModuleConfiguration::ModuleConfiguration(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}
ModuleConfiguration::~ModuleConfiguration()
{}

bool ModuleConfiguration::Start()
{
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

	active = true;
	return true;
}
update_status ModuleConfiguration::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleConfiguration::CleanUp()
{
	return true;
}

int ModuleConfiguration::GetFPS() {
	return fpsCap;
}


void ModuleConfiguration::Draw()
{

	if (ImGui::Begin("Configuration", &App->config->active, ImGuiWindowFlags_HorizontalScrollbar))
	ImGui::SetWindowPos(ImVec2(width * 0, height * 0 + 20));
	ImGui::SetWindowSize(ImVec2(width * 0.25, height * 0.75 - 20));
	{
		ImGui::Text("Configuration Menu");
		ImGui::Separator();
		ImGui::Separator();


		if (ImGui::CollapsingHeader("Application")) {
			ImGui::TextWrapped("App Name: Shiny Engine");
			ImGui::TextWrapped("Organization: UPC CITM");
			ImGui::Separator();
			ImGui::Separator();
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
			//sprintf_s(ms_title, 25, "Memory Consumption %.1f", mem_title[GRAPH_ARRAY_SIZE - 1]);
			ImGui::PlotHistogram("", mem_array, IM_ARRAYSIZE(mem_title), 30, mem_title, 0.0f, 1000000.0f, ImVec2(0, 80));

			//TotalReported memory
			ImGui::Text("Total Reported memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalReportedMemory);
			//TotalActual memory
			ImGui::Text("Total actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalActualMemory);
			//PeakReported memory
			ImGui::Text("Peak reported memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakReportedMemory);
			//PeakActual memory
			ImGui::Text("Peak actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakActualMemory);
			//AccumulatedReported memory
			ImGui::Text("Accumulated reported memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedReportedMemory);
			//AccumulatedActual memory
			ImGui::Text("Accumulated actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedActualMemory);
			//AccumulatedAllocUnit count
			ImGui::Text("Accumulated Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedAllocUnitCount);
			//TotalAllocUnit count
			ImGui::Text("Total Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalActualMemory);
			//PeakAllocUnit count
			ImGui::Text("Peak Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakAllocUnitCount);
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

			if (ImGui::Button("Apply"))
			{

				SDL_SetWindowSize(App->window->window, width, height);

				SDL_SetWindowBrightness(App->window->window, brightness);
			}

		}



		if (ImGui::CollapsingHeader("Hardware")) {
			App->hardware->Draw("Hardware");
		}

		if (ImGui::CollapsingHeader("Render"))
		{

			GLint polygonMode[2];
			glGetIntegerv(GL_POLYGON_MODE, polygonMode);
			if (polygonMode[0] == GL_LINE && polygonMode[1] == GL_LINE)
				wireframeMode = true;
			if (ImGui::Checkbox("Wireframe", &wireframeMode))
			{
				if (wireframeMode == true)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				if (wireframeMode == false)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			
		}

		if (ImGui::CollapsingHeader("Light")) {

			lightOn = App->renderer3D->lights[0].on;
			ImGui::Checkbox("On/Off", &lightOn);

			lightPos[0] = App->renderer3D->lights[0].position.x;
			lightPos[1] = App->renderer3D->lights[0].position.y;
			lightPos[2] = App->renderer3D->lights[0].position.z;
			ImGui::Text("Pos:");
			ImGui::SameLine();
			ImGui::InputFloat3("", lightPos);

			lightColour[0] = App->renderer3D->lights[0].ambient.r;
			lightColour[1] = App->renderer3D->lights[0].ambient.g;
			lightColour[2] = App->renderer3D->lights[0].ambient.b;
			lightColour[3] = App->renderer3D->lights[0].ambient.a;
			ImGui::Text("Color:");
			ImGui::SameLine();
			ImGui::ColorPicker4("", lightColour, 0);

			App->renderer3D->lights[0].Active(lightOn);
			App->renderer3D->lights[0].SetPos(lightPos[0], lightPos[1], lightPos[2]);
			App->renderer3D->lights[0].ambient.Set(lightColour[0], lightColour[1], lightColour[2], lightColour[3]);
			App->renderer3D->lights[0].Init();
		}

		//Input
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
	ImGui::End();
}