#include "GUI/Console.h"
#include "GUI/Hierarchy.h"
#include "GUI/Inspector.h"
#include "GUI/Stats.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

extern float DeltaTime;

Console* console;
Hierarchy* hierarchy;
Inspector* inspector;
Stats* stats;
bool doRenderGui;

//GUI Manager, handles the GUI system
void InitializeGUI()
{
	//Creates openGL3 Renderer
	CEGUI::OpenGL3Renderer& myRenderer = CEGUI::OpenGL3Renderer::bootstrapSystem();

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	//Creates resource paths
	rp->setResourceGroupDirectory("schemes", "Assets/CEGUI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "Assets/CEGUI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "Assets/CEGUI/fonts/");
	rp->setResourceGroupDirectory("layouts", "Assets/CEGUI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "Assets/CEGUI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "Assets/CEGUI/lua_scripts/");

	//Sets resource paths
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	//Sets my modified TaharezLook.scheme to the scheme
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

	//Makes default font a little smaller
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	//Reference to windowManager
	CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

	//Gets root window
	CEGUI::Window* rootWindow = windowManager.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);
	
	//Creates core GUI windows
	hierarchy = new Hierarchy(rootWindow, "RRHierarchy.layout");
	console = new Console(rootWindow, "RRConsole.layout");
	inspector = new Inspector(rootWindow, "RRInspector.layout");
	stats = new Stats(rootWindow, "RRStats.layout");

	//Registers events for console
	console->RegisterHandlers();
	hierarchy->RegisterHandlers();

	//Show GUI on start
	doRenderGui = true;
}

void RenderGUI()
{
	CEGUI::System::getSingleton().injectTimePulse(DeltaTime);
	CEGUI::System::getSingleton().renderAllGUIContexts();
}

void CleanupGUI()
{
	delete(hierarchy);
	delete(console);
	delete(inspector);
	delete(stats);

}