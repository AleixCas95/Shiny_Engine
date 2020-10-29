#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleConfiguration.h"
#include "ModuleShapes.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "ModuleGUI.h"
#include "ParShapes/par_shapes.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Start()
{
	LOG("Loading ImGui");

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	App->config->active = false;

	return true;
}

update_status ModuleGUI::Update(float dt)
{

	static bool show_test_window = false;

	// Console
	if (App->console->active == true) {
		App->console->Draw("Console");
	}

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	if (configActive == true) {
		ImGui::Begin("");
		ImGui::SetWindowSize(ImVec2(320, 689), ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos(ImVec2(App->window->width - App->window->width, App->window->height - App->window->height + 20));
		if (ImGui::SmallButton("Inspector")) {
			App->config->active = false;
			App->gui->showInspector = true;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Configuration")) {
			App->config->active = true;
			App->gui->showInspector = false;
		}
		ImGui::Separator();
		if (App->config->active == true) {
			App->config->Draw("test");
		}
		ImGui::End();
	}


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Show test window"))
			{
				if (show_test_window)
				{
					show_test_window = false;
				}
				else
					show_test_window = true;
			}

			ImGui::Checkbox("ShowConfig", &configActive);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			App->about->Draw();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Shapes"))
		{
			App->shape->Draw();
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}


	ImGui::Render();
	return UPDATE_CONTINUE;


}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return(UPDATE_CONTINUE);
}

bool ModuleGUI::CleanUp()
{
	LOG("Unloading ImGui");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}
