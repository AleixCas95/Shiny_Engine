#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleConfiguration.h"
#include "ModuleShapes.h"
#include "ComponentCamera.h"
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
	App->console->AddLog("Loading ImGui");

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	App->config->active = false;

	return true;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	ImGuizmo::BeginFrame();

	static bool show_test_window = false;

	
	if (show_test_window == true)
	{
		ImGui::ShowTestWindow();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			if (ImGui::MenuItem("Save"))
			{
				saveScenePopup = true;
			}
			if (ImGui::MenuItem("Load"))
			{
				loadScenePopup = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("ShowConfig", &configActive);
			ImGui::Checkbox("ShowInspector", &showInspector);
			ImGui::Checkbox("ShowHirearchy", &showHierarchy);
			ImGui::Checkbox("ShowConsole", &showConsole);
			ImGui::Checkbox("show_text_window", &show_test_window);
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
	
		if (configActive == true)
		{
			App->config->Draw();
		}
		if (showInspector == true)
		{
			App->inspector->Draw();
		}

		if (showConsole == true)
		{
			App->console->Draw("Console");
		}

		if (showHierarchy == true)
		{
			App->hierarchy->Draw();
		
		}
		//App->camera->compCamera->Inspector();

		ImGui::EndMainMenuBar();
	}
	
	if (App->config->wireframeMode == true) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
		
	ImGui::Render();

	if (App->config->wireframeMode == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	return UPDATE_CONTINUE;


}

update_status ModuleGUI::PostUpdate(float dt)
{
	
	return(UPDATE_CONTINUE);
}

bool ModuleGUI::CleanUp()
{
	App->console->AddLog("Unloading ImGui");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}
