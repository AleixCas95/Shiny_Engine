#include "Application.h"
#include "Globals.h"
#include "ModuleAbout.h"

ModuleAbout::ModuleAbout(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
ModuleAbout::~ModuleAbout()
{}

bool ModuleAbout::Start()
{
	return true;
}
update_status ModuleAbout::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleAbout::CleanUp()
{
	return true;
}


void ModuleAbout::Draw()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 100), "Shiny Engine");
	ImGui::Text("Shiny Engine is an educational project carried out by Aleix Castillo\n Arria nand Aitor Velez Tolosa. Setteled at UPC (CITM) Terrassa\n \n Hope you like it.");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0, 1, 1, 100), "Project links");
	if (ImGui::MenuItem("- Link to Shiny Engine repository"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/AleixCas95/Shiny_Engine", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::Separator();
	ImGui::Text("=== License ===");
	ImGui::TextWrapped("Shiny Engine is under MIT License, see LICENSE for more information.");
	if (ImGui::MenuItem("MIT License"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://opensource.org/licenses/MIT", 0, SW_SHOWMAXIMIZED);
	}

	
}