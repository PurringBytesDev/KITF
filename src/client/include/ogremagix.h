/*
-----------------------------------------------------------------------------
Filename:    ogremagix.h
-----------------------------------------------------------------------------
This is the heart of the base game
-----------------------------------------------------------------------------
*/
#ifndef __ogremagix_h_
#define __ogremagix_h_

#include "MagixApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


class ogremagixFrameListener : public MagixFrameListener, public OIS::MouseListener, public OIS::KeyListener
{
private:
	// are the pointers proper?
	SceneManager* mSceneMgr;
	MagixHandler *mMagixHandler;
	MagixPlayer* Mplayer;
public:
	ogremagixFrameListener(MagixHandler *magixHandler, SceneManager *sceneMgr, RenderWindow* win, Camera* cam) 
		: MagixFrameListener(magixHandler, win, cam, true, true),
		mSceneMgr(sceneMgr),
		mMagixHandler(magixHandler)
	{
		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
		mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);
	}

	bool frameStarted(const FrameEvent& evt)
	{
		// we should have an else case with an alert box or log
		bool frameStarted = MagixFrameListener::frameStarted(evt);

		if(frameStarted)
		{
			frameStarted = mMagixHandler->update(evt);

			// is this even related to input since this handle fog ? WTF ?
			if (mWindow->getViewport(0)->getBackgroundColour() != mSceneMgr->getFogColour())
			{
				mWindow->getViewport(0)->setBackgroundColour(mSceneMgr->getFogColour());
			}
		}

		// to inspect if frameStarted is NOT true : stop the game. (but we should have a log then)
		//if(!frameStarted)mMagixHandler->shutdown();

      return frameStarted;
	}

	// these should go in input the whole frame listening is a farce, these are not specifically related

	// MouseListener
    bool mouseMoved(const OIS::MouseEvent &e)
	{
		using namespace OIS;

		const MouseState &ms = e.state;
		const Real mouseSense = 0.5 + mMagixHandler->getMagixGUI()->getMouseSensitivity();

		//Camera view mode
		if(!mMagixHandler->getGameState()->isCinematic() && mMagixHandler->getGameState()->isInGame() && mMagixHandler->getCameraMode()==CAMERA_LOCKED)
		{
			if(mMagixHandler->getMagixGUI()->getUseWindowsCursor())
			{
				unsigned int tW,tH,tCD;
				int tL,tT;
				mWindow->getMetrics(tW,tH,tCD,tL,tT);
				SetCursorPos(tL+tW*0.5,tT+tH*0.5);
				//if(ShowCursor(false)<-1)ShowCursor(true); 
			}

			mMagixHandler->getMagixGUI()->showMouse(false);

			//Right button held, Zoom camera
			if( ms.buttonDown( MB_Right ) && !mMagixHandler->getCamera()->getIsFirstPerson())
			{
				if(mMagixHandler->getMagixGUI()->isTablet())
				{
					mMagixHandler->getCamera()->doCameraZoom((Math::Abs(ms.X.rel)>8000?0:-ms.Y.rel) * 0.0013 * mouseSense);
				}
				else
				{
					mMagixHandler->getCamera()->doCameraZoom(-ms.Y.rel * 0.13 * mouseSense);
				}
			}
			//Rotate camera
			else if(!mMagixHandler->getPlayer()->getIsLockedOn())
			{
				if(mMagixHandler->getMagixGUI()->isTablet())
				{
					mMagixHandler->getCamera()->yaw(Degree((Math::Abs(ms.X.rel)>8000?0:-ms.X.rel) * 0.0013 * mouseSense));
					mMagixHandler->getCamera()->pitch(Degree((Math::Abs(ms.Y.rel)>8000?0:-ms.Y.rel) * 0.0013 * mouseSense));
				}
				else
				{
					mMagixHandler->getCamera()->yaw(Degree(-ms.X.rel * 0.13 * mouseSense));
					mMagixHandler->getCamera()->pitch(Degree(-ms.Y.rel * 0.13 * mouseSense));
				}
			}
		}
		//Mouse cursor mode (actually : to be removed mode)
		else
		{
			const bool tDoZoom = ( (mMagixHandler->getCameraMode()==CAMERA_FREECURSORNEW && ms.buttonDown( MB_Left ) && ms.buttonDown( MB_Right ))
									|| (mMagixHandler->getCameraMode()==CAMERA_FREECURSOR && ms.buttonDown( MB_Middle )) );
			const bool tDoRotate = ( (mMagixHandler->getCameraMode()==CAMERA_FREECURSORNEW && ms.buttonDown( MB_Middle ))
									|| (mMagixHandler->getCameraMode()==CAMERA_FREECURSOR && ms.buttonDown( MB_Right ))
									|| (mMagixHandler->getCameraMode()==CAMERA_FREE && ms.buttonDown( MB_Right )));
			//zoom camera
			if(tDoZoom && !mMagixHandler->getGameState()->isCinematic() && mMagixHandler->getGameState()->isInGame())
			{
				if(mMagixHandler->getMagixGUI()->getUseWindowsCursor())
				{
					// todo : give better variable names (for map editor case since this is not going to be player no more)
					unsigned int tW,tH,tCD;
					int tL,tT;

					mWindow->getMetrics(tW, tH, tCD, tL, tT);
					SetCursorPos(tL + tW * 0.5, tT + tH * 0.5);
					//if(ShowCursor(false)<-1)ShowCursor(true); 
				}
				mMagixHandler->getMagixGUI()->showMouse(false);
				if(mMagixHandler->getMagixGUI()->isTablet())
				{
					mMagixHandler->getCamera()->doCameraZoom((Math::Abs(ms.X.rel)>8000?0:-ms.Y.rel) * 0.0013 * mouseSense);
				}
				else
				{
					mMagixHandler->getCamera()->doCameraZoom(-ms.Y.rel * 0.13 * mouseSense);
				}
			}
			//rotate view
			else if(tDoRotate && !mMagixHandler->getGameState()->isCinematic() && mMagixHandler->getGameState()->isInGame())
			{
				if(mMagixHandler->getMagixGUI()->getUseWindowsCursor())
				{
					unsigned int tW,tH,tCD;
					int tL,tT;
					mWindow->getMetrics(tW,tH,tCD,tL,tT);
					SetCursorPos(tL+tW*mMagixHandler->getMagixGUI()->getMouseX(),tT+tH*mMagixHandler->getMagixGUI()->getMouseY());
					//if(ShowCursor(false)<-1)ShowCursor(true); 
				}
				mMagixHandler->getMagixGUI()->showMouse(false);
				if(mMagixHandler->getMagixGUI()->isTablet())
				{
					mMagixHandler->getCamera()->yaw(Degree((Math::Abs(ms.X.rel)>8000?0:-ms.X.rel) * 0.0013 * mouseSense));
					mMagixHandler->getCamera()->pitch(Degree((Math::Abs(ms.Y.rel)>8000?0:-ms.Y.rel) * 0.0013 * mouseSense));
				}
				else
				{
					mMagixHandler->getCamera()->yaw(Degree(-ms.X.rel * 0.13 * mouseSense));
					mMagixHandler->getCamera()->pitch(Degree(-ms.Y.rel * 0.13 * mouseSense));
				}
			}
			//move the dayum cursor
			else
			{
				mMagixHandler->getMagixGUI()->showMouse(true);
				if(mMagixHandler->getMagixGUI()->getUseWindowsCursor())
				{
					//if(ShowCursor(true)>0)ShowCursor(false);
					unsigned int tW,tH,tCD;
					int tL,tT;
					tagPOINT lpPoint;
					mWindow->getMetrics(tW,tH,tCD,tL,tT);
					GetCursorPos(&lpPoint);
					const int tX = lpPoint.x-tL-4;
					const int tY = lpPoint.y-tT-30;
					mMagixHandler->getMagixGUI()->scrollMouse(tX,tY,tW,tH,true);
					if(tX<0 || tY<0 || tX>0&&(unsigned)tX>tW || tY>0&&(unsigned)tY>tH){if(ShowCursor(true)>0)ShowCursor(false);}
					else if(ShowCursor(false)<-1)ShowCursor(true);
				}
				else
				{
					// we don't have a tablet build this should be nuked from here
					if(mMagixHandler->getMagixGUI()->isTablet())
					{
						//tablet cursor always moves, see HERE
					}
					else
					{
						mMagixHandler->getMagixGUI()->scrollMouse( ms.X.rel * 1 * mouseSense, ms.Y.rel * 1 * mouseSense, mWindow->getWidth(), mWindow->getHeight());
					}
				}
			}
		}
		//HERE
		// this should gtfo into a void, this is an useless piece of code
		if(mMagixHandler->getMagixGUI()->isTablet() && !mMagixHandler->getMagixGUI()->getUseWindowsCursor())
		{
			const Real tXYRatio = Real(mWindow->getWidth())/Real(mWindow->getHeight());
			mMagixHandler->getMagixGUI()->scrollMouse( ms.X.rel * 0.015 * tXYRatio * mouseSense, ms.Y.rel * 0.015 * mouseSense, mWindow->getWidth(), mWindow->getHeight());
		}

		return true;
	}
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
	{
		using namespace OIS;

		if(mMagixHandler->getMagixGUI()->skipLogo())return true;

		mMagixHandler->getMagixGUI()->setLeftClick(e.state.buttonDown( MB_Left ));
		mMagixHandler->getMagixGUI()->setRightClick(e.state.buttonDown( MB_Right ));
		return true;
	}
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
	{
		using namespace OIS;

		mMagixHandler->getMagixGUI()->setLeftClick(e.state.buttonDown( MB_Left ));
		mMagixHandler->getMagixGUI()->setRightClick(e.state.buttonDown( MB_Right ));
		return true;
	}

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &e)
	{
		using namespace OIS;

		//Skip logo
		if(mMagixHandler->getMagixGUI()->skipLogo())
		{
			return true;
		}

		//Ingame controls
		if(!mMagixHandler->getGameState()->isCinematic() && mMagixHandler->getGameState()->isInGame() && mMagixHandler->getInputMode()==INPUT_CONTROL)
		{
			switch(e.key)
			{
				//Player controls
				// we should see about if azerty is pressed too ? or OIS understand Z/W indiscriminately ?
			case KC_W:
				if (mMagixHandler->getCameraMode() != CAMERA_FREE)
				{
					mMagixHandler->getPlayer()->up = true;
					if (mMagixHandler->getPlayer()->getAutoRun())
					{
						mMagixHandler->getPlayer()->toggleAutoRun();
					}
				}
				else
				{
					mMagixHandler->getCamera()->heldMove(DIR_FORWARD, true);
				}
				break;
			case KC_S:
				if (mMagixHandler->getCameraMode() != CAMERA_FREE)
				{
					mMagixHandler->getPlayer()->down = true;
					if (mMagixHandler->getPlayer()->getAutoRun())mMagixHandler->getPlayer()->toggleAutoRun();
				}
				else mMagixHandler->getCamera()->heldMove(DIR_BACK, true);
				break;
			case KC_A:
				if (mMagixHandler->getCameraMode() != CAMERA_FREE)mMagixHandler->getPlayer()->left = true;
				else mMagixHandler->getCamera()->heldMove(DIR_LEFT, true);
				break;
			case KC_D:
				if (mMagixHandler->getCameraMode() != CAMERA_FREE)mMagixHandler->getPlayer()->right = true;
				else mMagixHandler->getCamera()->heldMove(DIR_RIGHT, true);
				break;
			case KC_SPACE:
				if (mMagixHandler->getMagixGUI()->isDoubleJumpOn() && mMagixHandler->getPlayer()->isReadyToDoubleJump)mMagixHandler->getPlayer()->doDoubleJump();
				mMagixHandler->getPlayer()->setJump(true);
				break;
			case KC_LSHIFT: mMagixHandler->getPlayer()->setIsWalking(!mMagixHandler->getPlayer()->getAutoWalk());
				break;
			case KC_UP:
				mMagixHandler->getCamera()->heldTurn(DIR_UP, true);
				break;
			case KC_DOWN:
				mMagixHandler->getCamera()->heldTurn(DIR_DOWN, true);
				break;
			case KC_LEFT:
				mMagixHandler->getCamera()->heldTurn(DIR_LEFT, true);
				break;
			case KC_RIGHT:
				mMagixHandler->getCamera()->heldTurn(DIR_RIGHT, true);
				break;
			case KC_PGUP:
				if (mMagixHandler->getCameraMode() == CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_UP, true);
				}
				else
				{
					mMagixHandler->getPlayer()->ascend = true;
				}
				break;
			case KC_PGDOWN:
				if (mMagixHandler->getCameraMode() == CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_DOWN, true);
				}
				else
				{
					mMagixHandler->getPlayer()->descend = true;
				}
				break;
			case KC_Z:
				mMagixHandler->getPlayer()->doSit();
				break;
			case KC_X:
				mMagixHandler->getPlayer()->doSideLay();
				break;
			case KC_C:
				if (mMagixHandler->getPlayer()->getWingEnt())
				{
					mMagixHandler->getPlayer()->toggleAntiGravity();
				}
				break;
			case KC_NUMPAD8:
				mMagixHandler->getPlayer()->up = true;
				break;
			case KC_NUMPAD2:
				mMagixHandler->getPlayer()->down = true;
				break;
			case KC_NUMPAD4:
				mMagixHandler->getPlayer()->left = true;
				break;
			case KC_NUMPAD6:
				mMagixHandler->getPlayer()->right = true;
				break;
			case KC_NUMPAD9:
				mMagixHandler->getPlayer()->ascend = true;
				break;
			case KC_NUMPAD3:
				mMagixHandler->getPlayer()->descend = true;
				break;
			case KC_MINUS:
			case KC_SUBTRACT:
				mMagixHandler->getCamera()->changeSpeed(0.5);
				break;
			case KC_EQUALS:
			case KC_ADD:
				mMagixHandler->getCamera()->changeSpeed(2);
				break;
			//case KC_F5:
			case KC_F6:
			case KC_F7:
			case KC_F8:
			case KC_F9:
			case KC_F10:
			case KC_F11:
			case KC_F12:
				if (!mMagixHandler->getGameState()->isCampaign())
				{
					MagixExternalDefinitions* def = mMagixHandler->getExternalDefinitions();
					mMagixHandler->getPlayer()->setEmote(def->getHotkeyF(e.key - (e.key >= KC_F11 ? KC_F11 - 6 : KC_F5)), def);
					mMagixHandler->getNetworkManager()->sendEmote(def->getHotkeyF(e.key - (e.key >= KC_F11 ? KC_F11 - 6 : KC_F5)));
					mMagixHandler->getInputManager()->hotkeyHeld(e.key);
				}
				break;
			case KC_F:
				mMagixHandler->getPlayer()->toggleStance(mMagixHandler->getExternalDefinitions());
				break;
			case KC_E:
				if(mMagixHandler->getCameraMode() == CAMERA_LOCKED)
				{
					mMagixHandler->getUnitManager()->doLockOn();
				}
				break;
			// seems to be key numbers on top of text keys ? to validate.
			case KC_1:
				mMagixHandler->getPlayer()->setCurrentSkill(0);
				mMagixHandler->getMagixGUI()->updateSkillText(0);
				break;
			case KC_2:
				/*
				case KC_3:
				case KC_4:
				case KC_5:
				case KC_6:
				case KC_7:
				case KC_8:
				case KC_9:
				case KC_0:
				*/
				mMagixHandler->getPlayer()->setCurrentSkill(mMagixHandler->getPlayer()->getNextSkill());
				mMagixHandler->getMagixGUI()->updateSkillText(mMagixHandler->getPlayer()->getCurrentSkill());
				break;
				//Toggle controls
				// this is going to be removed soon to keep only 2 modes
			case KC_F1:
				mMagixHandler->getCamera()->setCameraMode(CAMERA_LOCKED);
				mMagixHandler->getMagixGUI()->showMouse(false);
				mMagixHandler->getMagixGUI()->showSelectPanel(false);
				if(mMagixHandler->getMagixGUI()->isOptionsVisible())
				{
					mMagixHandler->getMagixGUI()->toggleShowOptions();
				}
				//mMagixHandler->getMagixGUI()->getChatManager()->message("Locked Chase Camera: Left Click = Attack, Right Click (hold) = Zoom.");
				break;
			case KC_F2:
				mMagixHandler->getCamera()->setCameraMode(CAMERA_FREECURSOR);
				mMagixHandler->getMagixGUI()->showMouse(true);
				//mMagixHandler->getMagixGUI()->getChatManager()->message("Free Chase Camera: Left Click = Select, Right Click (hold) = Rotate, Middle Click (hold) = Zoom.");
				break;
			case KC_F3:
				// this is likely the camera mode we want to neutralize.
				mMagixHandler->getCamera()->setCameraMode(CAMERA_FREE);
				mMagixHandler->getMagixGUI()->showMouse(true);
				mMagixHandler->getPlayer()->up = false;
				mMagixHandler->getPlayer()->down = false;
				mMagixHandler->getPlayer()->left = false;
				mMagixHandler->getPlayer()->right = false;
				mMagixHandler->getPlayer()->ascend = false;
				mMagixHandler->getPlayer()->descend = false;
				//mMagixHandler->getMagixGUI()->getChatManager()->message("Free Camera: Left Click = Select, Right Click (hold) = Rotate.");
				break;
			case KC_F4:
				mMagixHandler->getCamera()->setCameraMode(CAMERA_LOCKED, true);
				mMagixHandler->getMagixGUI()->showMouse(false);
				mMagixHandler->getMagixGUI()->showSelectPanel(false);
				
				if(mMagixHandler->getMagixGUI()->isOptionsVisible())
				{
					mMagixHandler->getMagixGUI()->toggleShowOptions();
				}
				
				if(!mMagixHandler->getPlayer()->getIsLockedOn())
				{
					mMagixHandler->getPlayer()->setAutoTrackObject(0);
				}
				//mMagixHandler->getMagixGUI()->getChatManager()->message("First-person Camera: Left Click = Attack.");
				break;
			case KC_F5:
				mMagixHandler->getCamera()->setCameraMode(CAMERA_FREECURSORNEW);
				mMagixHandler->getMagixGUI()->showMouse(true);
				//mMagixHandler->getMagixGUI()->getChatManager()->message("Free Chase Camera 2: Left Click = Select, Right Click = Target, Middle Click (hold) = Rotate, Left+Right Click (hold) = Zoom.");
				break;
			case KC_ESCAPE:
				mMagixHandler->getMagixGUI()->toggleShowOptions();
				if (mMagixHandler->getCameraMode() == CAMERA_LOCKED)
				{
					mMagixHandler->getCamera()->setCameraMode(CAMERA_FREECURSOR);
					mMagixHandler->getMagixGUI()->showMouse(true);
				}
				break;
				//case KC_R: if(mKeyboard->isKeyDown(KC_LCONTROL))mMagixHandler->resetScene();
				// break;
				//case KC_P: mMagixHandler->getGameState()->togglePause();
				// break;
				//case KC_L: showDebugOverlay(!isStatsOn);
				// break;
			case KC_INSERT:
				mMagixHandler->getMagixGUI()->toggleIsTablet(true); 
				break;
			case KC_RETURN:
			case KC_NUMPADENTER:
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getMagixGUI()->toggleInputMode(true);
				}
				break;
			case KC_LCONTROL:
				mMagixHandler->getPlayer()->doCrouch(true);
				break;
			case KC_RCONTROL:
				mMagixHandler->getMagixGUI()->toggleRunMode();
				break;
			case KC_LMENU:
			case KC_RMENU:
				if (!mMagixHandler->getPlayer()->getIsLockedOn())
				{
					mMagixHandler->getPlayer()->setFreeLook(!mMagixHandler->getPlayer()->getFreeLook());
				}					
				break;
			case KC_GRAVE:
				mMagixHandler->getMagixGUI()->toggleChatChannel();
				break;
			case KC_TAB:
				mMagixHandler->getMagixGUI()->toggleShowGUI();
				break;
			case KC_H: 
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getMagixGUI()->toggleShowHomeBox();
				}
				break;
			case KC_B:
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getMagixGUI()->toggleShowBioBox();
				}
				break;
			case KC_N:
				if(!mMagixHandler->getGameState()->isCampaign())mMagixHandler->getMagixGUI()->toggleShowFriendBox();	
				break;
			case KC_M:
				mMagixHandler->getMagixGUI()->toggleShowMiniMap();
				break;
			case KC_J:
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getMagixGUI()->toggleShowEmoteBox();
				}
				break;
			case KC_I:
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getMagixGUI()->toggleShowItemBox();
				}
				break;
			case KC_P:
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getMagixGUI()->toggleShowPartyBox();
				}
				break;
			case KC_R:
				mMagixHandler->getPlayer()->toggleAutoRun();
				break;
			case KC_CAPITAL:
				mMagixHandler->getPlayer()->toggleAutoWalk();
				break;
			case KC_SYSRQ:
				mWindow->writeContentsToTimestampedFile("screenshot", ".PNG");
				mMagixHandler->getMagixGUI()->getAlertBox()->showAlert("Screenshot saved", 0.5, 0.5);
				break;
			default:
				break;
			}
		}
		//Text input
		else if(mMagixHandler->getInputMode()==INPUT_TEXT)
		{
			switch (e.key)
			{
				case KC_ESCAPE:
					mMagixHandler->getMagixGUI()->toggleShowOptions();
					if(mMagixHandler->getGameState()->isInGame() && mMagixHandler->getCameraMode()==CAMERA_LOCKED )
					{
						mMagixHandler->getCamera()->setCameraMode(CAMERA_FREECURSOR);
						mMagixHandler->getMagixGUI()->showMouse(true);
					}
				break;
				case KC_RETURN:
				case KC_NUMPADENTER:
					mMagixHandler->getMagixGUI()->toggleInputMode(true);
					break;

				default:
					mMagixHandler->getInputManager()->keyHeld(mKeyboard,e.key,e.text,true);
				break;
			}
		}
		//Outgame controls
		else
		{
			switch (e.key)
			{
				case KC_ESCAPE:
					mMagixHandler->getMagixGUI()->toggleShowOptions();
					break;
				//case KC_L: showDebugOverlay(!isStatsOn);
				// break;
				case KC_INSERT:
					mMagixHandler->getMagixGUI()->toggleIsTablet(true);
					break;
				case KC_SYSRQ:
					mWindow->writeContentsToTimestampedFile("screenshot",".PNG");
					mMagixHandler->getMagixGUI()->getAlertBox()->showAlert("Screenshot saved",0.5,0.5);
					break;

				default:
				break;
			}
		}

		/// key debug ?
		//mMagixHandler->getMagixGUI()->getAlertBox()->showAlert(StringConverter::toString(e.key),0.5,0.5);

		return mContinue;
	}
    bool keyReleased(const OIS::KeyEvent &e)
	{
		using namespace OIS;

		//Ingame controls
		if(mMagixHandler->getGameState()->isInGame() && !mMagixHandler->getGameState()->isCinematic())
		switch (e.key)
		{
			//Player controls
			case KC_W:
				if(mMagixHandler->getCameraMode()==CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_FORWARD, false);
				}
				else
				{
					mMagixHandler->getPlayer()->up = false;
				}
			break;
			case KC_S:
				if(mMagixHandler->getCameraMode()==CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_BACK, false);
				}
				else
				{
					mMagixHandler->getPlayer()->down = false;
				}
			break;
			case KC_A:
				if(mMagixHandler->getCameraMode()==CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_LEFT, false);
				}
				else
				{
					mMagixHandler->getPlayer()->left = false;
				}
			break;
			case KC_D:
				if(mMagixHandler->getCameraMode()==CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_RIGHT, false);
				}
				else
				{
					mMagixHandler->getPlayer()->right = false;
				}
			break;
			case KC_SPACE:
				if(mMagixHandler->getPlayer()->getJump())
				{
					mMagixHandler->getPlayer()->isReadyToDoubleJump = true;
				}
				mMagixHandler->getPlayer()->setJump(false);
				break;
			case KC_LSHIFT:
				mMagixHandler->getPlayer()->setIsWalking(mMagixHandler->getPlayer()->getAutoWalk());
				break;
			case KC_LCONTROL:
				mMagixHandler->getPlayer()->doCrouch(false);
				break;
			case KC_UP:
				mMagixHandler->getCamera()->heldTurn(DIR_UP,false);
				break;
			case KC_DOWN:
				mMagixHandler->getCamera()->heldTurn(DIR_DOWN,false);
				break;
			case KC_LEFT:
				mMagixHandler->getCamera()->heldTurn(DIR_LEFT,false);
				break;
			case KC_RIGHT:
				mMagixHandler->getCamera()->heldTurn(DIR_RIGHT,false);
				break;
			case KC_PGUP:
				if(mMagixHandler->getCameraMode()==CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_UP, false);
				}
				else
				{
					mMagixHandler->getPlayer()->ascend = false;
				}
				break;
			case KC_PGDOWN:
				if(mMagixHandler->getCameraMode()==CAMERA_FREE)
				{
					mMagixHandler->getCamera()->heldMove(DIR_DOWN, false);
				}
				else
				{
					mMagixHandler->getPlayer()->descend = false;
				}
				break;
			case KC_NUMPAD8:
				mMagixHandler->getPlayer()->up = false;
				break;
			case KC_NUMPAD2:
				mMagixHandler->getPlayer()->down = false;
				break;
			case KC_NUMPAD4:
				mMagixHandler->getPlayer()->left = false;
				break;
			case KC_NUMPAD6:
				mMagixHandler->getPlayer()->right = false;
				break;
			case KC_NUMPAD9:
				mMagixHandler->getPlayer()->ascend = false;
				break;
			case KC_NUMPAD3:
				mMagixHandler->getPlayer()->descend = false;
				break;
			case KC_F5:
			case KC_F6:
			case KC_F7:
			case KC_F8:
			case KC_F9:
			case KC_F10:
			case KC_F11:
			case KC_F12:
				if(!mMagixHandler->getGameState()->isCampaign())
				{
					mMagixHandler->getInputManager()->hotkeyHeld(0);
				}
				break;

			default:
			break;
		}
		//Text input
		if(mMagixHandler->getInputMode()==INPUT_TEXT)
		{
			mMagixHandler->getInputManager()->keyHeld(mKeyboard,e.key,e.text,false);
		}

		return true;
	}
};

class ogremagixApp : public MagixApplication
{
private:
	MagixHandler *mMagixHandler;

	public:
		ogremagixApp():
		  MagixApplication(mMagixHandler)
    {
		mMagixHandler = new MagixHandler();
	}

	~ogremagixApp()
	{
		//Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
		//overlayManager->destroyAll();
		delete mMagixHandler;
	}

protected:
	virtual void createCamera(void)
	{
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->setNearClipDistance( 0.1 );
		mCamera->setFarClipDistance( 20000 );
	}

    virtual bool configure(void)
    {
		// Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
		
		if(mRoot->showConfigDialog())
        {
			// If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
            mWindow = mRoot->initialise(true, "KITF - Keeping IT Free");
			mWindow->setDeactivateOnFocusChange(false);
			
			// Let's add a nice window icon
			HWND hwnd;
			mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
			LONG iconID = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
			
			// for 32 bit its GCL_HICON
			SetClassLongPtr(hwnd, GCLP_HICON, iconID);

            return true;
        }
        else
        {
            return false;
        }
    }

	// if its an override, whats special then in the setup ? bloom ? blur ?
	// Just override the mandatory create scene method
	virtual void createScene(void)
	{
		ColourValue fadeColour = DEFAULT_FOG_COLOUR;
		mSceneMgr->setFog( FOG_LINEAR, fadeColour, .0001, 500, 1500);
		mWindow->getViewport(0)->setBackgroundColour(fadeColour);

		mMagixHandler->initialize(mSceneMgr,mWindow);
		mMagixHandler->getExternalDefinitions()->initializeCapabilities(mRoot->getRenderSystem()->getCapabilities());

		// Infinite far plane?
		if(mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
		{
			mCamera->setFarClipDistance(0);
		}

		if(mMagixHandler->getExternalDefinitions()->hasVertexProgram && mMagixHandler->getExternalDefinitions()->hasFragmentProgram)
		{
			CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), "Bloom"); 
			CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), "MotionBlur");
		}
	}

	virtual void destroyScene()
	{
		mMagixHandler->shutdown();
	}
	
	// not inherently bad but we need to get the framelistener out
	// Create new frame listener
	void createFrameListener(void)
	{
		mFrameListener= new ogremagixFrameListener(mMagixHandler, mSceneMgr, mWindow, mCamera);
		mRoot->addFrameListener(mFrameListener);
	}
};
#endif