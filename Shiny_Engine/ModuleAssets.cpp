#include "Application.h"
#include "imgui/imgui.h"
#include "ModuleResources.h"
#include "Resources.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleFiles.h"
#include "ModuleGui.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleAssets.h"
#include "ModuleConsole.h"

ModuleAssets::ModuleAssets(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "assets";
	active = true;
	pos_x = 70, pos_y = 70, width = 300, height = 500;

}


ModuleAssets::~ModuleAssets()
{


}

void ModuleAssets::Draw()
{
	//Set window position and size
	if (resize) {
		int x, y;
		App->window->GetWinSize(x, y);
		height = y - 550;
		ImGui::SetNextWindowPos(ImVec2(0.0f, y - height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
		resize = false;
	}


	//Window
	ImGui::Begin("Assets", &active, ImGuiWindowFlags_None);

	//Directory tree
	ImGui::BeginChild(1, { (float)width - 15, (float)height - 70 }, true);

	vector<string> file_list;
	vector<string> dir_list;

	file_list.clear();
	dir_list.clear();
	App->files->DiscoverFiles("Assets/", file_list, dir_list);

	GestionDirectoryTree(dir_list);
	ImGui::EndChild();

	if (ImGui::Button("Open", { 80, 20 })) {
		OpenScene();
	}
	ImGui::SameLine();
	if (ImGui::Button("Put to GameObject", { 140, 20 })) {
		PutOnGameObject();
	}
	ImGui::SameLine();

	ImGui::End();
}

void ModuleAssets::GestionDirectoryTree(vector<string> dir)
{
	int id = 0;

	for (int i = 0; i < dir.size(); ++i) {

		string this_dir = "Assets/" + dir[i];

		vector<string> file_list;
		vector<string> dir_list;

		App->files->DiscoverFiles(this_dir.c_str(), file_list, dir_list);

		if (dir[i] == "Meshes")
			DrawAssetTree(file_list, dir[i], id, true, File_Mesh);
		else if (dir[i] == "Textures")
			DrawAssetTree(file_list, dir[i], id, true, File_Material);
		else if (dir[i] == "Scripts")
			DrawAssetTree(file_list, dir[i], id, true, File_Script);
		else
			DrawAssetTree(file_list, dir[i], id, true, File_Unknown);

		id++;
	}

}

void ModuleAssets::DrawAssetTree(vector<string> files, string name, int& id, bool is_directory, FileType ft)
{
	ImGui::PushID(id);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;

	if (!is_directory && ft != File_Mesh)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (id == focused_node)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx((void*)(intptr_t)id, flags, name.c_str())) {

		//File nodes
		if (!is_directory) {
			if (ImGui::IsItemClicked()) {
				focused_node = id;
				foc_node_name = name;
			}

			DrawAssetTreeDiferentFiles(files, name, id, ft);
		}

		//Directory nodes
		if (is_directory) {
			for (int i = 0; i < files.size(); ++i) {
				FileType ft = App->files->DetermineFileType((char*)files[i].c_str());
				if (ft != File_Meta) { //Don't show metas
					id++;
					DrawAssetTree(files, files[i], id, false, ft);
				}
			}
		}


		ImGui::TreePop();
	}

	ImGui::PopID();
}

void ModuleAssets::DrawAssetTreeDiferentFiles(vector<string> files, string name, int& id, FileType ft)
{
	switch (ft) {
	case File_Mesh: {
		//Draw the mesh nodes inside de mesh scene (.fbx, .obj) node
		map<string, map<UID, string>>::iterator it = mesh_scenes.find(name);

		if (it != mesh_scenes.end()) {
			map<UID, string> mesh_scn = it->second;
			for (std::map<UID, string>::const_iterator it = mesh_scn.begin(); it != mesh_scn.end(); ++it) {
				id++;
				DrawAssetTree(files, it->second, id, false, No_Extension);

				//Get this mesh name and uuid
				if (ImGui::IsItemClicked())
					foc_mesh = mesh_scn;

				//Initiate drag and drop, to put a mesh to a component mesh
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					UID uuid = it->first;
					ImGui::SetDragDropPayload("Assets_Mesh", &uuid, sizeof(UID));

					ImGui::EndDragDropSource();
				}
			}
		}
	}

				  break;

	case File_Script: {
		map<string, UID>::iterator it = scripts.find(name);

		if (it != scripts.end()) {
			//Initiate drag and drop, to put a script to a component graph script
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				UID uuid = it->second;
				ImGui::SetDragDropPayload("Assets_Script", &uuid, sizeof(UID));

				ImGui::EndDragDropSource();
			}
		}
	}
					break;
	}
}

void ModuleAssets::OpenScene()
{
	FileType ft = App->files->DetermineFileType((char*)foc_node_name.c_str());
	switch (ft) {
	case File_Mesh: {
		string path = "Assets/Meshes/" + foc_node_name;
		App->scene_serialization->LoadSceneFromMesh(path.c_str());
	}
				  break;

	case File_Scene: {
		string path = "Assets/Scenes/" + foc_node_name;
		App->scene_serialization->LoadScene(path.c_str());
	}
				   break;

	default:
		//App->gui->AddLog("You can't charge this file type into the scene");
		break;

	}
}

void ModuleAssets::PutOnGameObject()
{
	FileType ft = App->files->DetermineFileType((char*)foc_node_name.c_str());
	switch (ft) {
	case No_Extension:
		for (std::map<UID, string>::const_iterator it = foc_mesh.begin(); it != foc_mesh.end(); ++it) {
			if (foc_node_name == it->second) {
				if (App->scene->GetFocusedGameObject())
					App->scene->AssignMeshToGameObject(it->first);
				else {
					GameObject* go = App->scene->CreateGameObjectByMesh(it->first);
					go->SetName(it->second.c_str());
				}
			}
		}
		break;
	case File_Material: {
		if (foc_tex_uuid != 0)
			App->scene->AssignTexToGameObject(foc_tex_uuid);
	}
					  break;
	default:
		App->console->AddLog("You cannot put this to a GameObject as a component");
		break;

	}
}

void ModuleAssets::FillMeshScenesMap(map<string, map<UID, string>> mesh_scenes)
{
	this->mesh_scenes = mesh_scenes;
}

void ModuleAssets::FillTexturesMap(map<string, UID> textures)
{
	this->textures = textures;
}

void ModuleAssets::FillScriptsMap(map<string, UID> scripts)
{
	this->scripts = scripts;
}

void ModuleAssets::AddToScriptsMap(std::string name, UID uuid)
{
	scripts[name] = uuid;
}
