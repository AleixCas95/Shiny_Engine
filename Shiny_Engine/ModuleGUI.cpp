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

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


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
			ImGui::Checkbox("ShowCamera", &showCamera);
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

		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("AABB", "F1"))
				App->renderer3D->drawBoxes = !App->renderer3D->drawBoxes;
			if (ImGui::MenuItem("Textures", "F2"))
				App->renderer3D->DebugTextures();
			if (ImGui::MenuItem("Culling", "F3"))
				App->renderer3D->culling = !App->renderer3D->culling;

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

		if (showCamera)
		{
			ImGui::Begin("Camera", &App->gui->showCamera);

			App->camera->compCamera->Inspector();

			ImGui::End();
		}

		if (saveScenePopup)
		{
			ImGui::OpenPopup("Save Scene Here");
			saveScenePopup = false;
		}

		if (ImGui::BeginPopup("Save Scene Here"))
		{
			char buf[64];
			sprintf_s(buf, 64, lastSceneName.data());
			if (ImGui::InputText("Scene Name", buf, IM_ARRAYSIZE(buf)))
			{
				lastSceneName = buf;
			}

			ImGui::SameLine();

			if (ImGui::Button("SAVE"))
			{
				App->gobject->SaveScene(lastSceneName.data());

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("CANCEL"))
			{
				lastSceneName = "Scene";
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (loadScenePopup)
		{
			ImGui::OpenPopup("Load");
			loadScenePopup = false;
		}

		if (ImGui::BeginPopup("Load"))
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Drag the scene file to load");
			ImGui::EndPopup();
		}

		if (ImGui::Begin("State Buttons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			std::string play(Time::gameState == GameState::EDITOR ? "Play" : "Stop");

			if (ImGui::Button(play.c_str()))
			{
				if (App->renderer3D->play_cam)
				{
					Time::gameState = Time::gameState == GameState::EDITOR ? GameState::PLAYING : GameState::EDITOR;

					App->renderer3D->current_cam = Time::gameState == GameState::PLAYING ? App->renderer3D->play_cam : App->camera->compCamera;
				}
			}
		}


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
