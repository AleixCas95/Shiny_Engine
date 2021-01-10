#ifndef __ModuleAssets_H__
#define __ModuleAssets_H__

#include "Application.h"
#include "Module.h"
#include <vector>
#include <map>
#include <string>

using namespace std;
typedef unsigned int UID;
enum FileType;

class ModuleAssets : public Module
{
public:
	ModuleAssets(Application* app, bool start_enabled = true);
	virtual ~ModuleAssets();

	void Draw();

	void GestionDirectoryTree(vector<string> dir);

	//Draw asset tree
	void DrawAssetTree(vector<string> files, string name, int& id, bool is_directory, FileType ft);
	void DrawAssetTreeDiferentFiles(vector<string> files, string name, int& id, FileType ft);

	void OpenScene();
	void PutOnGameObject();

	void FillMeshScenesMap(map<string, map<UID, string>> mesh_scenes);
	void FillTexturesMap(map <string, UID> textures);
	void FillScriptsMap(map <string, UID> scripts);
	void AddToScriptsMap(std::string name, UID uuid);

private:
	//Focused
	int focused_node = -1;
	std::string foc_node_name;
	map<UID, string> foc_mesh;
	UID foc_tex_uuid;

	map<string, map<UID, string>> mesh_scenes;
	map<string, UID> textures;
	map<string, UID> scripts;

	bool active = true;
	char* name = nullptr;
	int pos_x, pos_y, width, height;
	bool resize = true;
};

#endif