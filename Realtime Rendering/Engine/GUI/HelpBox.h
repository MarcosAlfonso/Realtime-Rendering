#pragma once
#include "GUIBase.h"

//Inspector, GUI Window that handles component editing with current object
class HelpBox : public GUIBase
{
public:
	HelpBox(CEGUI::Window* rootWindow, std::string path);
	CEGUI::Listbox* Label;

};

