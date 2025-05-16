#ifndef __MagixNetworkManager_h_
#define __MagixNetworkManager_h_

#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "MagixNetworkDefines.h"
#include "MagixChatManager.h"
#include "MagixAlertBox.h"

class MagixNetworkManager
{
protected:
	MagixExternalDefinitions *mDef;
	MagixGameStateManager *mGameStateManager;
	MagixUnitManager *mUnitManager;
	MagixWorld *mWorld;
	MagixSkyManager *mSkyManager;
	MagixChatManager *mChatManager;
	MagixAlertBox *mAlertBox;
	RakPeerInterface *mPeer;
	String mDebugText;
	bool acceptedFlag;
	Real updateCount;
public:
	MagixNetworkManager()
	{
		mDef = 0;
		mGameStateManager = 0;
		mUnitManager = 0;
		mWorld = 0;
		mSkyManager = 0;
		mChatManager = 0;
		mAlertBox = 0;
		mPeer = 0;
		mDebugText = "";
		acceptedFlag = false;
		updateCount = 0;
	}
	~MagixNetworkManager()
	{
		if(mPeer)
		{
			if(mPeer->IsActive())disconnect();
			RakNetworkFactory::DestroyRakPeerInterface(mPeer);
		}
	}
	void initialize(MagixExternalDefinitions *def, MagixGameStateManager *gamestateMgr, MagixUnitManager *unitMgr, MagixWorld *world, MagixSkyManager *skyMgr, MagixChatManager *chatMgr, MagixAlertBox *alertBox)
	{
		mDef = def;
		mGameStateManager = gamestateMgr;
		mUnitManager = unitMgr;
		mWorld = world;
		mSkyManager = skyMgr;
		mChatManager = chatMgr;
		mAlertBox = alertBox;
		mPeer = RakNetworkFactory::GetRakPeerInterface();
	}
	void connect()
	{
		String tIP = mDef->serverIP;
		mPeer->Startup(1,30,&SocketDescriptor(), 1);
		mPeer->Connect(tIP.c_str(), SERVER_PORT, SERVER_PASSWORD, (int)strlen(SERVER_PASSWORD));
	}
	void disconnect()
	{
		mPeer->Shutdown(300);
	}
	void reset()
	{
	}
	void update(const FrameEvent evt)
	{
		if(acceptedFlag) updateCount -= evt.timeSinceLastFrame;
		if(acceptedFlag && updateCount<=0)
		{
			updateCount += 0.25;
			sendMovePosition();
			if(mUnitManager->getPlayer()->getFreeLook())sendLookQuaternion();
			sendFaceDirection();
			sendActionFlags();
		}

		Packet *packet = mPeer->Receive();
		while(packet)
		{
			switch (getPacketIdentifier(packet))
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				sendNewPlayer();
				sendNewPlayerColours();
				break;
			case ID_WORLDINFO:
				processWorldInfo(packet);
				mUnitManager->resetPlayerPosition();
				sendPosition();
				acceptedFlag = true;
				updateCount = 1;
				mGameStateManager->setGameState(GAMESTATE_STARTGAME);
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
			case ID_INVALID_PASSWORD:
				mAlertBox->showAlert("Our connection request has failed.",0.5,0.5);
				mWorld->loadWorld("Default");
				mWorld->update();
				mGameStateManager->setGameState(GAMESTATE_STARTGAME);
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				mAlertBox->showAlert("The server is full.",0.5,0.5);
				mWorld->loadWorld("Default");
				mWorld->update();
				mGameStateManager->setGameState(GAMESTATE_STARTGAME);
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				mAlertBox->showAlert("We have been disconnected.",0.5,0.5);
				break;
			case ID_CONNECTION_LOST:
				mAlertBox->showAlert("Connection lost.",0.5,0.5);
				break;
			case ID_DEBUGTEXT: mDebugText = getPacketData(packet); break;
			case ID_CHAT: processChat(packet); break;
			case ID_NEWPLAYER: processNewPlayer(packet); break;
			case ID_NEWPLAYERCOLOURS: processNewPlayerColours(packet); break;
			case ID_POSITION: processPosition(packet); break;
			case ID_NEWPLAYERUPDATE: processAndSendNewPlayerUpdate(packet); break;
			case ID_REQUESTMAPTIME: processAndSendMapTime(packet); break;
			case ID_PLAYERDISCONNECTED: processPlayerDisconnected(packet); break;
			case ID_MOVEPOSITION: processMovePosition(packet); break;
			case ID_LOOKQUATERNION: processLookQuaternion(packet); break;
			case ID_FACEDIRECTION: processFaceDirection(packet); break;
			case ID_ACTIONFLAGS: processActionFlags(packet); break;
			case ID_EMOTE: processEmote(packet); break;
			case ID_ATTACK: processAttack(packet); break;
			default:
				mDebugText = "Message with identifier " + StringConverter::toString(int(getPacketIdentifier(packet))) + " has arrived.";
				break;
			}

			mPeer->DeallocatePacket(packet);
			packet = mPeer->Receive();
		}
	}
	String getDebugText()
	{
		return mDebugText;
	}
	unsigned char getPacketIdentifier(Packet *p)
	{
		if (p==0)
			return 255;

		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		{
			assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
			return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
		}
		else
			return (unsigned char) p->data[0];
	}
	unsigned char getPacketOwner(Packet *p)
	{
		if (p==0)
			return 0;
		if(p->length<2) return 0;

		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		{
			assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
			return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long) + 1];
		}
		else
			return (unsigned char) p->data[1];
	}
	const String getPacketData(Packet *p)
	{
		if(p->length < 3)return "";
		return String((char*)p->data).erase(0,2);
	}
	void processChat(Packet *p)
	{
		if(getPacketOwner(p)==SERVER_ID)
		{
			mChatManager->message(getPacketData(p));
		}
		else
		{
			mChatManager->say(mUnitManager,mUnitManager->getUnitByIndex(getPacketOwner(p)),getPacketData(p));
		}
	}
	void sendChat(String caption)
	{
		caption.insert(0,"00");
		caption[0] = ID_CHAT;
		caption[1] = char(TOKEN_NULL);
		mPeer->Send(caption.c_str(),(int)caption.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processNewPlayer(Packet *p)
	{
		String tData = getPacketData(p);
		vector<String> tParam = StringUtil::split(tData,";",6);
		if(tParam.size()==6)
		{
			MagixIndexedUnit *tUnit = mUnitManager->createUnit(getPacketOwner(p),tParam[1],tParam[2],tParam[3],tParam[4],tParam[5]);
			mUnitManager->createNameTag(tUnit,tParam[0]);
			tUnit->setAlliance(ALLIANCE_FRIEND);
			mChatManager->message(tParam[0] + " has arrived.");
		}
	}
	void sendNewPlayer(unsigned char target=TOKEN_NULL)
	{
		String tData = "00";
		tData[0] = ID_NEWPLAYER;
		tData[1] = target;
		tData = tData + mUnitManager->getPlayer()->getNameTag()->getCaption() + ";"
				+ mUnitManager->getPlayer()->getMeshString() + ";"
				+ mUnitManager->getPlayer()->getMatGroupName();
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processNewPlayerColours(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;
		if(tUnit->getMatGroupName()!="CustomMat")return;

		String tData = getPacketData(p);
		tUnit->setColours(tData);
	}
	void sendNewPlayerColours(unsigned char target=TOKEN_NULL)
	{
		if(mUnitManager->getPlayer()->getMatGroupName()!="CustomMat")return;
		String tData = "00";
		tData[0] = ID_NEWPLAYERCOLOURS;
		tData[1] = target;
		tData = tData + mUnitManager->getPlayer()->getColourString();
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processPosition(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		tUnit->setPosition(StringConverter::parseVector3(getPacketData(p)));
		tUnit->resetTarget();
		mUnitManager->clampUnitToTerrain(tUnit);
		//mDebugText += "\nPlayer " + StringConverter::toString(int(getPacketOwner(p))) + " set position: " + getPacketData(p);
	}
	void sendPosition(unsigned char target=TOKEN_NULL)
	{
		String tData = "00";
		tData[0] = ID_POSITION;
		tData[1] = target;
		tData = tData + StringConverter::toString(mUnitManager->getPlayer()->getPosition());
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processMovePosition(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		tUnit->setTarget(StringConverter::parseVector3(getPacketData(p)));
		//mDebugText = "Player " + StringConverter::toString(int(getPacketOwner(p))) + " move position: " + getPacketData(p);
	}
	void sendMovePosition()
	{
		String tData = "00";
		tData[0] = ID_MOVEPOSITION;
		tData[1] = char(TOKEN_NULL);
		tData = tData + StringConverter::toString(mUnitManager->getPlayer()->getPosition());
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, UNRELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processLookQuaternion(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		tUnit->setLookDirection(StringConverter::parseQuaternion(getPacketData(p)));
		//mDebugText += "Player " + StringConverter::toString(int(getPacketOwner(p))) + " look quaternion: " + getPacketData(p);
	}
	void sendLookQuaternion()
	{
		String tData = "00";
		tData[0] = ID_LOOKQUATERNION;
		tData[1] = char(TOKEN_NULL);
		tData = tData + StringConverter::toString(mUnitManager->getPlayer()->getLookDirection());
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, UNRELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processFaceDirection(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		tUnit->setYaw(StringConverter::parseAngle(getPacketData(p)));
		//mDebugText += "Player " + StringConverter::toString(int(getPacketOwner(p))) + " look quaternion: " + getPacketData(p);
	}
	void sendFaceDirection()
	{
		String tData = "00";
		tData[0] = ID_FACEDIRECTION;
		tData[1] = char(TOKEN_NULL);
		tData = tData + StringConverter::toString(mUnitManager->getPlayer()->getYaw());
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, UNRELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processActionFlags(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		vector<String> tFlag = StringUtil::split(getPacketData(p),";");
		if(tFlag.size()==5)
		{
			tUnit->setJump(StringConverter::parseBool(tFlag[0]));
			tUnit->setFreeLook(StringConverter::parseBool(tFlag[1]));
			tUnit->setIsWalking(StringConverter::parseBool(tFlag[2]));
			if(StringConverter::parseBool(tFlag[3]))tUnit->doSit();
			if(StringConverter::parseBool(tFlag[4])!=tUnit->getIsStance())tUnit->toggleStance(mDef);
		}
	}
	void sendActionFlags()
	{
		String tData = "00";
		tData[0] = ID_ACTIONFLAGS;
		tData[1] = char(TOKEN_NULL);
		tData = tData + (mUnitManager->getPlayer()->getJump()?"1":"0") + ";" +
				(mUnitManager->getPlayer()->getFreeLook()?"1":"0") + ";" +
				(mUnitManager->getPlayer()->getIsWalking()?"1":"0") + ";" +
				(mUnitManager->getPlayer()->isSitting()?"1":"0") + ";" +
				(mUnitManager->getPlayer()->getIsStance()?"1":"0");
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, UNRELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processEmote(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		tUnit->setEmote(getPacketData(p),mDef);
	}
	void sendEmote(String emote)
	{
		String tData = "00";
		tData[0] = ID_EMOTE;
		tData[1] = char(TOKEN_NULL);
		tData = tData + emote;
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processAndSendNewPlayerUpdate(Packet *p)
	{
		sendNewPlayer(getPacketOwner(p));
		sendNewPlayerColours(getPacketOwner(p));
		sendPosition(getPacketOwner(p));
	}
	void processPlayerDisconnected(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;
		mChatManager->message(tUnit->getName() + " has left.");
		mUnitManager->deleteUnit(tUnit);
		//mDebugText = "Player " + StringConverter::toString(int(getPacketOwner(p))) + " has disconnected.";
	}
	void processWorldInfo(Packet *p)
	{
		String tData = getPacketData(p);
		vector<String> tPart = StringUtil::split(tData,";",2);
		if(tPart.size()>0)mSkyManager->setDayTime(StringConverter::parseReal(tPart[0]));
		if(tPart.size()>1)mWorld->loadWorld(tPart[1]);
	}
	void processAndSendMapTime(Packet *p)
	{
		String tData = "00";
		tData[0] = ID_REQUESTMAPTIME;
		tData[1] = getPacketOwner(p);
		tData = tData + StringConverter::toString(mSkyManager->getDayTime());
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	void processAttack(Packet *p)
	{
		MagixIndexedUnit *tUnit = mUnitManager->getUnitByIndex(getPacketOwner(p));
		if(!tUnit)return;

		tUnit->forceAttack(getPacketData(p));
	}
	void sendAttack(const String &attack)
	{
		String tData = "00";
		tData[0] = ID_ATTACK;
		tData[1] = char(TOKEN_NULL);
		tData = tData + attack;
		mPeer->Send(tData.c_str(),(int)tData.length()+1,HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
};

#endif