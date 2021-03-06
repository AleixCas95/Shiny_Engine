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

	ImGui::Begin(title);
	ImGui::SetWindowPos(ImVec2(App->config->width * 0, App->config->height * 0.75));
	ImGui::SetWindowSize(ImVec2(App->config->width * 0.75, App->config->height * 0.25));
	
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

	// Write in console
	for (int i = 0; i < items.Size; i++) {
		const char* currentItem = items[i];
		ImVec4 colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		if (strstr(currentItem, "[error]")) {
			colour = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		}
		else if (strncmp(currentItem, "# ", 2) == 0) {
			colour = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		}
		ImGui::PushStyleColor(ImGuiCol_Text, colour);
		ImGui::TextUnformatted(currentItem);
		ImGui::PopStyleColor();
	}
	if (scrollToBottom == true) {
		ImGui::SetScrollHere();
	}
	scrollToBottom = false;


	ImGui::EndChild();

	ImGui::End();
}