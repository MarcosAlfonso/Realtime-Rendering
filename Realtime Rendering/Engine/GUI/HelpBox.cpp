#include "HelpBox.h"
#include "GUIBase.h"

#include <CEGUI/CEGUI.h>

//Inspector, GUI Window that handles component editing with current object
HelpBox::HelpBox(CEGUI::Window* rootWindow, std::string path) : GUIBase(rootWindow, path)
{
	Label = static_cast<CEGUI::Listbox*>(myWindow->getChild("HelpBoxLabel"));

	Label->appendText("WASD + R. Mouse: Move Camara\n");
	Label->appendText("Ctrl, Space: Camara Height\n");
	Label->appendText("Q,E: Spawn physics primitives\n");
	Label->appendText("P: Pause Physics\n");
	Label->appendText("G: Hide GUI\n");

}