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


void InitializeGUI()
{
	CEGUI::OpenGL3Renderer& myRenderer = CEGUI::OpenGL3Renderer::bootstrapSystem();

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());


	rp->setResourceGroupDirectory("schemes", "Assets/CEGUI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "Assets/CEGUI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "Assets/CEGUI/fonts/");
	rp->setResourceGroupDirectory("layouts", "Assets/CEGUI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "Assets/CEGUI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "Assets/CEGUI/lua_scripts/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");


	CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* rootWindow = windowManager.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);
	
	hierarchy = new Hierarchy(rootWindow, "RRHierarchy.layout");
	console = new Console(rootWindow, "RRConsole.layout");
	inspector = new Inspector(rootWindow, "RRInspector.layout");
	stats = new Stats(rootWindow, "RRStats.layout");

	console->RegisterHandlers();

	doRenderGui = true;
}



void RenderGUI()
{
	CEGUI::System::getSingleton().injectTimePulse(DeltaTime);
	CEGUI::System::getSingleton().renderAllGUIContexts();
}