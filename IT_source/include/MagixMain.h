#ifndef __MagixMain_h_
#define __MagixMain_h_

#include <Ogre.h>
#include <OIS/OIS.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
}
#endif

#include "MagixWorld.h"


using namespace Ogre;

class MagixMain
{
protected:
	Root *mRoot;
	RenderWindow *mWindow;
	Viewport *mViewport;
	SceneManager *mSceneMgr;
	Camera *mCamera;

	OIS::InputManager *mInputManager;
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;

	String mResourcePath;

	MagixWorld *mWorld;

public:
	MagixMain()
	{
		mRoot = 0;
		mInputManager = 0;
		// Provide a nice cross platform solution for locating the configuration files
		// On windows files are searched for in the current working directory, on OS X however
		// you must provide the full path, the helper function macBundlePath does this for us.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = macBundlePath() + "/Contents/Resources/";
#else
		mResourcePath = "";
#endif
		mWorld = 0;
	}
	~MagixMain()
	{
		if(mInputManager)
		{
			mInputManager->destroyInputObject(mKeyboard);
			mInputManager->destroyInputObject(mMouse);
			OIS::InputManager::destroyInputSystem(mInputManager);
		}

		if(mWorld)delete mWorld;
		if(mRoot)delete mRoot;
	}
	void go()
	{
		String pluginsPath;
		// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
		pluginsPath = mResourcePath + "plugins.cfg";
#endif
		
        mRoot = new Root(pluginsPath, 
            mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

        setupResources();
		
		if(mRoot->showConfigDialog())
		{
			mWindow = mRoot->initialise(true, "Magix");
			mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
			createCamera();

			ResourceGroupManager::getSingleton().addResourceLocation("../../media/trees", "FileSystem");
			ResourceGroupManager::getSingleton().addResourceLocation("../../media/terrains", "FileSystem");
			ResourceGroupManager::getSingleton().addResourceLocation("../../media/grass", "FileSystem");
			ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

			initializeOIS();

			load();
			run();
		}

		unload();
	}
	void setupResources()
	{
		// Load resource paths from config file
		ConfigFile cf;
		cf.load(mResourcePath + "resources.cfg");

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app,
				// In order to make things portable on OS X we need to provide
				// the loading with it's own bundle path location
				ResourceGroupManager::getSingleton().addResourceLocation(
					String(macBundlePath() + "/" + archName), typeName, secName);
#else
				ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
#endif
			}
		}
	}

	void createCamera()
	{
		mCamera = mSceneMgr->createCamera("PlayerCam");
		SceneNode *camPivotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamPivotNode");
		SceneNode *camNode = camPivotNode->createChildSceneNode("CameraNode");

		mCamera->setNearClipDistance( 0.1 );
		mCamera->setFarClipDistance( 10000 );

		mViewport = mWindow->addViewport(mCamera);
        mViewport->setBackgroundColour(ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio( Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	}
	void initializeOIS()
	{
		using namespace OIS;
		size_t windowHnd;
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		mInputManager = InputManager::createInputSystem(windowHnd);

		mKeyboard = (Keyboard*)mInputManager->createInputObject(OISKeyboard, false);
		mMouse = (Mouse*)mInputManager->createInputObject(OISMouse, false);
	}
	void load()
	{
		mWorld = new MagixWorld;
		mWorld->initialize(mSceneMgr);
	}
	void unload()
	{
	}
	void run()
	{
		bool bCont = true;
		while(bCont)
		{
			//Handle windows events
			WindowEventUtilities::messagePump();

			//Update frame
			bCont = processInput();
			render();

			//Exit immediately if the window is closed
			if(mWindow->isClosed())break;
		}
	}
	void render()
	{
		mWorld->update();
		mRoot->renderOneFrame();
	}
	bool processInput()
	{
		bool bCont = true;



		return bCont;
	}
};

#endif