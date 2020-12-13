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
	ImGui::Separator();

	ImGui::Text("Libraries used");
	if (ImGui::MenuItem("- ImGui (v1.52)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/ocornut/imgui", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Parson"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://kgabis.github.io/parson/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- MathGeoLib (v2.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://clb.demon.fi/MathGeoLib/nightly/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- mmgr"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Devil (v1.7.8). Download on the Astat's comment."))
	{
		ShellExecuteA(0, 0, "chrome.exe", " http://openil.sourceforge.net", 0, SW_SHOWMAXIMIZED);
		ShellExecuteA(0, 0, "chrome.exe", "http://www.stratos-ad.com/forums/index.php?topic=9897.0", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Assimp (v3.3.1)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://assimp.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Glew (v2.0.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://glew.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("PCG 0.94"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.pcg-random.org/download.html", 0, SW_SHOWMAXIMIZED);
	}


	if (ImGui::MenuItem("- Glew (v2.0.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://glew.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- SDL (v2.0.6)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://www.libsdl.org/", 0, SW_SHOWMAXIMIZED);
	}


	if (ImGui::BeginMenu("- OpenGL info"))
	{
		ImGui::Text("Vendor: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", glGetString(GL_VENDOR));

		ImGui::Text("Renderer: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", glGetString(GL_RENDERER));

		ImGui::Text("OpenGL version supported: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", glGetString(GL_VERSION));

		ImGui::Text("GLSL: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ImGui::EndMenu();
	}
	ImGui::Separator();
	ImGui::Spacing();
	// Name of the Author
	ImGui::Text("Authors:");
	ImGui::Spacing();
	ImGui::Bullet();
	ImGui::SameLine();
	if (ImGui::MenuItem("Aleix Castillo Arria"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/AleixCas95", 0, SW_SHOWMAXIMIZED);
	}
	ImGui::Bullet(); ImGui::SameLine();
	if (ImGui::MenuItem("Aitor Velez Tolosa"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/AitorVelez", 0, SW_SHOWMAXIMIZED);
	}
	
}