#include "Application.h"
#include "Globals.h"
#include "Panel_About.h"

PanelAbout::PanelAbout() : Panel("About")
{
	active = false;
}

PanelAbout::~PanelAbout()
{}


void PanelAbout::Draw()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 100), "=== Shiny Engine ===");
	ImGui::Text("Shiny Engine is a educational project made at CITM Terrassa.\n Hope you like it.");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0, 1, 1, 100), "Github link");
	if (ImGui::MenuItem("- Link to Shiny Engine repository"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/AleixCas95/Shiny_Engine", 0, SW_SHOWMAXIMIZED);
	}
	ImGui::Separator();

	//license

	ImGui::TextColored(ImVec4(0, 1, 1, 100), "MIT License");
	ImGui::Text("Copyright <2020> <Aleix Castillo Arrià / Aitor Vélez Tolosa>");
	ImGui::Text("\n");
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy ");
	ImGui::Text("of this softwareand associated documentation files(the Software), to deal");
	ImGui::Text ("in the Software without restriction, including without limitation the rights");
	ImGui::Text(" to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
	ImGui::Text("copies of the Software, and to permit persons to whom the Software is furnished");
	ImGui::Text("to do so, subject to the following conditions :");
	ImGui::Text("\n");
	ImGui::Text("The above copyright noticeand this permission notice shall be included in all");
	ImGui::Text("copies or substantial portions of the Software.");
	ImGui::Text("\n");
	ImGui::Text("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,");
	ImGui::Text("INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A");
	ImGui::Text("PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT");
	ImGui::Text("HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION");
	ImGui::Text("OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE");
	ImGui::Text("OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
}