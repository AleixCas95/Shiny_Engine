#include "Application.h"
#include "Globals.h"
#include "ModuleConsole.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

ModuleConsole::ModuleConsole(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ClearLog();
	memset(inputBuf, 0, sizeof(inputBuf));
	AddLog("Welcome to ImGui!");
}
ModuleConsole::~ModuleConsole()
{}

bool ModuleConsole::Start()
{
	active = true;
	return true;
}
update_status ModuleConsole::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleConsole::CleanUp()
{
	ClearLog();
	return true;
}


void ModuleConsole::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	items.push_back(Strdup(fmt));
	scrollToBottom = true;
}


void ModuleConsole::ClearLog()
{
	if (items.Size > 0) {
		for (int i = 0; i < items.Size; i++) {
			free(items[i]);
		}
	}
	items.clear();
	scrollToBottom = true;
}


void ModuleConsole::Draw(const char* title)
{

	/*::Begin(title);

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	
	if (ImGui::SmallButton("Clear")) {
		ClearLog();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Scrool to bottom")) {
		scrollToBottom = true;
	}
	ImGui::Separator();

	ImGui::BeginChild(("Scroll"), ImVec2{ 0,-ImGui::GetItemsLineHeightWithSpacing() }, false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::Selectable("Clear")) {
			ClearLog();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();

	ImGui::End();*/
}