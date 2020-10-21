#include "Application.h"
#include "Globals.h"
#include "Panel_Console.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

PanelConsole::PanelConsole() : Panel("Console")
{
	active = true;
}
PanelConsole::~PanelConsole()
{}


void PanelConsole::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{

}
void PanelConsole::ClearLog()
{

}

void PanelConsole::Draw()
{

	ImGui::Begin("Console");

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	ImGui::Separator();

	ImGui::End();
}