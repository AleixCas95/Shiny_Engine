#ifndef __PanelConsole_H__
#define __PanelConsole_H__

#include "PanelGUI.h"
#include "Globals.h"
#include "imGUI\imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	~PanelConsole();

	void AddLog(const char*, ...) IM_FMTARGS(2);
	void ClearLog();

	void Draw() override;

};

#endif 
