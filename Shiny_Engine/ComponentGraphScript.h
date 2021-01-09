#pragma once
#ifndef __ComponentGraphScript__H__
#define __ComponentGraphScript__H__

#include "Component.h"
#include <string>
class ComponentGraphScript : public Component
{
public:
	ComponentGraphScript(Application* app_parent, GameObject* parent,uint script_num);
	~ComponentGraphScript();

	void DrawInspector();
	void Save(JSON_Array* comp_array) const;
	void Load(JSON_Object* comp_obj);

	//typedef unsigned int scriptType;
	scriptType scriptNum = 0;
	scriptType uuid_script;
private:
	//const char *uuid_script;
	bool has_script = false;
	char script_name[30] = "scripttest";
	std::vector<GameObject*> gobjects;
	std::vector<scriptType> uuidsLoad;
};

#endif