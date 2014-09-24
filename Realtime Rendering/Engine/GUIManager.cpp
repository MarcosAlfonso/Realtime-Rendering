#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

extern float DeltaTime;

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

	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");


	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* myRoot = wmgr.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

	CEGUI::FrameWindow* fWnd = static_cast<CEGUI::FrameWindow*>(
		wmgr.createWindow("AlfiskoSkin/FrameWindow", "testWindow"));

	CEGUI::Window* EditBox = wmgr.createWindow("AlfiskoSkin/Editbox", "OurDialog_Editbox");


	myRoot->addChild(fWnd);
	fWnd->addChild(EditBox);

	// position a quarter of the way in from the top-left of parent.
	fWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));
	// set size to be half the size of the parent
	fWnd->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));

	fWnd->setText("Hello World!");

}

void RenderGUI()
{
	CEGUI::System::getSingleton().injectTimePulse(DeltaTime);
	CEGUI::System::getSingleton().renderAllGUIContexts();
}