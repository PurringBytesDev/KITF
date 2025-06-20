#include "OgreResourceGroupManager.h"
#include "OgreException.h"
#include "OgreOverlay.h"
#include "OgreOverlayManager.h"
#include "OgreRenderWindow.h"
#include "fontTextureHelper.h"

using namespace Ogre;

/*
Based heavily off of Ogre's ExampleLoadingBar.h
Description: Defines an example loading progress bar which you can use during 
startup, level changes etc to display loading progress. 
IMPORTANT: Note that this progress bar relies on you having the OgreCore.zip 
package already added to a resource group called 'Bootstrap' - this provides
the basic resources required for the progress bar and will be loaded automatically.
*/
class MagixLoadingBar : public ResourceGroupListener
{
protected:
	RenderWindow* mWindow;
	Overlay* mLoadOverlay;
	Real mInitProportion;
	unsigned short mNumGroupsInit;
	unsigned short mNumGroupsLoad;
	Real mProgressBarMaxSize;
	Real mProgressBarScriptSize;
	Real mProgressBarInc;
	OverlayElement* mLoadingBarElement;
	OverlayElement* mLoadingDescriptionElement;
	OverlayElement* mLoadingCommentElement;

public:
	MagixLoadingBar() {}
	virtual ~MagixLoadingBar(){}

	/** Show the loading bar and start listening.
	@param window The window to update 
	@param numGroupsInit The number of groups you're going to be initialising
	@param numGroupsLoad The number of groups you're going to be loading
	@param initProportion The proportion of the progress which will be taken
		up by initialisation (ie script parsing etc). Defaults to 0.7 since
		script parsing can often take the majority of the time.
	*/
	virtual void start(RenderWindow* window, unsigned short numGroupsInit = 1, unsigned short numGroupsLoad = 1, Real initProportion = 0.70f)
	{
		mWindow = window;
		mNumGroupsInit = numGroupsInit;
		mNumGroupsLoad = numGroupsLoad;
		mInitProportion = initProportion;

		// We need to pre-initialise the 'Bootstrap' group so we can use
		// the basic contents in the loading screen
		// nb : was this bootstrap in IT ?
		ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");

		OverlayManager& omgr = OverlayManager::getSingleton();
		mLoadOverlay = (Overlay*)omgr.getByName("Core/LoadOverlay");

		if(!mLoadOverlay)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"Cannot find loading overlay", "MagixLoadingBar::start");
		}

		mLoadOverlay->show();

		// Save links to the bar and to the loading text, for updates as we go
		mLoadingBarElement = omgr.getOverlayElement("Core/LoadPanel/Bar/Progress");
		mLoadingCommentElement = omgr.getOverlayElement("Core/LoadPanel/Comment");
		mLoadingDescriptionElement = omgr.getOverlayElement("Core/LoadPanel/Description");

		OverlayElement* barContainer = omgr.getOverlayElement("Core/LoadPanel/Bar");
		mProgressBarMaxSize = barContainer->getWidth();
		mLoadingBarElement->setWidth(0);

		// self is listener
		ResourceGroupManager::getSingleton().addResourceGroupListener(this);
	}

	/** Hide the loading bar and stop listening. 
	*/
	virtual void finish(void)
	{
		// hide loading screen
		mLoadOverlay->hide();
		
		// Note : the new font texture & def are generated next to the game app for now
		// eventually turn into an independant tool with only minimal media.

		// after ressource load
		// todo : replace by other fonts
		bool defaultVal = false;

		// example converted font was "dead walking font" but it's basically a font you have in gui.fontdef originally
		FontGen((String)"DeadWalking", defaultVal);
		
		// Unregister listener
		ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
	}

	// FontGen changes : we need to call only in tools or user context, or we switch boolean for a manual generation.
	// Remember : a font need to be defined to be baked !Generated files will be next to game binary, you will need to add 
	// the new font def and update your overlay / UI / Whatever
	void FontGen(String &fontName, bool &askedByUser)
	{
		if(!askedByUser)
		{
			// we are not in a tool or button clicked context
			return;
		}
		// use only for debugging ?
		//generateAllFontTextures();

		// example: i want to use the TTF font Cyberbit in my application:
		fontCacheInit(fontName);

		// from now you can use font Cyberbit_cache in your application :)
	}

	// ResourceGroupListener callbacks
	void resourceGroupScriptingStarted(const String& groupName, size_t scriptCount)
	{
		assert(mNumGroupsInit > 0 && "You stated you were not going to init "
			"any groups, but you did! Divide by zero would follow...");
		// Lets assume script loading is 70%
		mProgressBarInc = mProgressBarMaxSize * mInitProportion / (Real)scriptCount;
		mProgressBarInc /= mNumGroupsInit;
		mLoadingDescriptionElement->setCaption("Parsing scripts...");
		mWindow->update();
	}

	void scriptParseStarted(const String& scriptName, bool &skipThisScript)
	{
		mLoadingCommentElement->setCaption(scriptName);
		mWindow->update();
	}

	void scriptParseEnded(const String& scriptName, bool skipped)
	{
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() + mProgressBarInc);
		mWindow->update();
	}

	void resourceGroupScriptingEnded(const String& groupName)
	{
	}

	void resourceGroupLoadStarted(const String& groupName, size_t resourceCount)
	{
		assert(mNumGroupsLoad > 0 && "You stated you were not going to load "
			"any groups, but you did! Divide by zero would follow...");
		mProgressBarInc = mProgressBarMaxSize * (1-mInitProportion) / 
			(Real)resourceCount;
		mProgressBarInc /= mNumGroupsLoad;
		mLoadingDescriptionElement->setCaption("Loading resources...");
		mWindow->update();
	}

	void resourceLoadStarted(const ResourcePtr& resource)
	{
		mLoadingCommentElement->setCaption(resource->getName());
		mWindow->update();
	}

	void resourceLoadEnded(void)
	{
	}

	void worldGeometryStageStarted(const String& description)
	{
		mLoadingCommentElement->setCaption(description);
		mWindow->update();
	}

	void worldGeometryStageEnded(void)
	{
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() + mProgressBarInc);
		mWindow->update();
	}

	void resourceGroupLoadEnded(const String& groupName)
	{
	}

	Ogre::DataStreamPtr resourceLoading(const String &name, const String &group, Resource *resource)
	{
		return Ogre::DataStreamPtr();
	}

	bool resourceCollision(ResourcePtr &resource, ResourceManager *resourceManager)
	{
		return false;
	}
};

