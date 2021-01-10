#pragma once
#ifndef __ComponentGraphScript__H__
#define __ComponentGraphScript__H__

#include "Component.h"
#include <string>
class ComponentGraphScript : public Component
{
public:
	ComponentGraphScript(Application* app_parent, GameObject* parent, uint script_num);
	~ComponentGraphScript();

	void DrawInspector();
	void Save(JSON_Array* comp_array) const;
	void Load(JSON_Object* comp_obj);

	void ForceAddReferenceToBlackboard(GameObject* ref);
	uint GetCompSriptNum() const;
	std::vector<GameObject*> GetBlackboard()const;

	uint CreateNewTimer();
	void IncrementTimer(uint idx, float dt);
	uint GetNumTimers() const;
	float GetTimer(uint idx) const;
	void LoadBlackBoard();


	//typedef unsigned int scriptType;
	scriptType scriptNum = 0;
	scriptType uuid_script;
private:
	//const char *uuid_script;
	bool has_script = false;
	char script_name[30] = "scripttest";
	std::vector<GameObject*> gobjects;
	std::vector<scriptType> uuidsLoad;
	uint script_num;

	std::vector<float> timers;
};

#endif