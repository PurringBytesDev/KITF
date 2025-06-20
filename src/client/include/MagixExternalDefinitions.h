#ifndef __MagixExternalDefinitions_h_
#define __MagixExternalDefinitions_h_

#include "MagixNetworkDefines.h"
#include "GameConfig.h"
#include <OgreStreamSerialiser.h>
#include <OgreException.h>
#include "Ogre.h"
#include "MagixStructs.h"
#include "MagixUtils.h"

// if aint used then can be removed ?
//using namespace std;
using namespace Ogre;

typedef vector<std::pair<String, String>>::type CampaignEvent;
typedef vector<CampaignEvent>::type CampaignEventList;

#define MAX_HOTKEYF 8
// Bruteforced all structs to a secondary file. 
// migration to the .cpp & other files in progress.

class MagixExternalDefinitions
{
protected:
	String lastBioName;
	String hotkeyF[MAX_HOTKEYF];
public:
	// new
	KITF_Utils::MagixUtils mMagixUtils;
	bool isAdmin;
	bool isMod;
	bool isModOn;
	ColourValue adminAuraColour;
	bool hasVertexProgram;
	bool hasFragmentProgram;
	bool pagedGeometryOn;
	bool pagedGeometryForced;
	Real viewDistance;
	unsigned short maxHeads;
	unsigned short maxManes;
	unsigned short maxTails;
	unsigned short maxWings;
	unsigned short maxTufts;
	unsigned short maxBodyMarks;
	unsigned short maxHeadMarks;
	unsigned short maxTailMarks;
	vector<String>::type headMesh;
	vector<String>::type maneMesh;
	vector<String>::type tailMesh;
	vector<String>::type wingMesh;
	vector<String>::type tuftMesh;
	unsigned short maxEmotes;
	vector<String>::type emoteName;
	vector<String>::type emoteAnims;
	vector<Real>::type emoteEyelidState;
	unsigned short maxItems;
	vector<String>::type itemMesh;
	vector<String>::type itemBone;
	vector<String>::type itemName;
	vector<bool>::type itemIsPrivate;
	vector<bool>::type itemHasOffset;
	vector<bool>::type itemHasAltAnim;
	vector<String>::type itemParticle;
	vector<String>::type itemParticleOffset;
	vector<bool>::type itemParticleOnNode;
	vector<Attack>::type attackList;
	vector<Critter>::type critterList;
	
	MagixExternalDefinitions()
	{
		lastBioName = "";
		isAdmin = false;
		isMod = false;
		isModOn = false;
		adminAuraColour = ColourValue(1, 1, 1);
		pagedGeometryOn = false;
		pagedGeometryForced = false;
		viewDistance = 1;
		hasVertexProgram = false;
		hasFragmentProgram = false;
		maxHeads = 0;
		maxManes = 0;
		maxTails = 0;
		maxWings = 0;
		maxTufts = 0;
		maxBodyMarks = 0;
		maxHeadMarks = 0;
		maxTailMarks = 0;
		maxEmotes = 0;
		maxItems = 0;
	}
	~MagixExternalDefinitions()
	{
	}
	void initialize();
	void initializeCapabilities(const RenderSystemCapabilities* capabilities);

	// a decision has to be taken, this is either a setting / configs loader OR a mesh / assets loader
	void loadUnitMeshes(const String& filename);

	void loadUnitEmotes(const String& filename)
	{
		maxEmotes = 0;
		emoteName.clear();
		emoteAnims.clear();
		emoteEyelidState.clear();

		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(!inFile.good())
		{
			Ogre::LogManager::getSingleton().logMessage("IT INITIALIZE ERROR: Unable to open file " + filename);
			return;
		}
		inFile.seekg(0, std::ios::end);
		tSize = (long)inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, '#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String>::type tPart = StringUtil::split(tData, "[#");
		maxEmotes = int(tPart.size());
		for (int i = 0; i < maxEmotes; i++)
		{
			const vector<String>::type tLine = StringUtil::split(tPart[i], "\n", 3);
			if(tLine.size() == 3)
			{
				String tName = tLine[0];
				tName.erase(tName.find_first_of("]"), 1);
				emoteName.push_back(tName);
				emoteAnims.push_back(tLine[1]);
				emoteEyelidState.push_back(StringConverter::parseReal(tLine[2]));
			}
		}
	}

	///
	/// Utilities to move out
	/// 
	
	// this is char manager only
	bool isRestricted(const short& headID, const short& maneID)
	{
		if(headID == 0 || headID == 4)
		{
			if(maneID == 2 || maneID == 4 || maneID == 7 || maneID == 8 || maneID == 9 || maneID == 10 || maneID == 14 || maneID == 15 || maneID == 16 || maneID == 17 || maneID == 18 || maneID == 19 || maneID == 20 || maneID == 21 || maneID == 22 || maneID == 23 || maneID == 24)
			{
				return true;
			}
		}
		else if(headID == 1 || headID == 2 || headID == 3 || headID == 5)
		{
			if(maneID == 1 || maneID == 3 || maneID == 5 || maneID == 6 || maneID == 11 || maneID == 12 || maneID == 13 || maneID == 21 || maneID == 23)
			{
				return true;
			}
		}

		return false;
	}

	bool isRestrictedTuft(const short& tuftID)
	{
		if(tuftID == 8)
		{
			return true;
		}

		return false;
	}

	// this is ingame logic it should be separate
	void decrypt(String& input)
	{
		for (int i = 0; i < (int)input.length(); i++)
		{
			input[i] = input[i] - 10;
		}
	}

	void encrypt(String& input)
	{
		for (int i = 0; i < (int)input.length(); i++)
		{
			input[i] = input[i] + 10;
		}
	}

	const String getMapName(const String& name)
	{
		const vector<String>::type tMapName = StringUtil::split(name, ";", 1);
		return (tMapName.size() > 0 ? tMapName[0] : name);
	}
	/// <summary>
	/// end of Utilities
	/// </summary>
	
	void loadSettings(bool& isTablet, Real& mouseSensitivity, Real& musicVol, Real& guiVol, Real& soundVol, bool& isShadowsOn, bool& isBloom, String& username, bool& generalCharname, bool& localUsername, bool& channelBlink, bool& runMode, bool& doubleJumpOn, bool& autoLipSync, bool& useWindowsCursor)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open("Settings.cfg", std::ifstream::in);
		if(inFile.good())
		{
			inFile.seekg(0, std::ios::end);
			tSize = (long)inFile.tellg();
			inFile.seekg(0, std::ios::beg);
			tBuffer = new char[tSize];
			strcpy(tBuffer, "");
			inFile.getline(tBuffer, tSize, '#');
			inFile.close();
			tData = tBuffer;
			delete[] tBuffer;
		}

		const vector<String>::type tLine = StringUtil::split(tData, "\n#", 16);
		if(tLine.size() >= 7)
		{
			isTablet = StringConverter::parseBool(tLine[0]);
			mouseSensitivity = StringConverter::parseReal(tLine[1]);
			musicVol = StringConverter::parseReal(tLine[2]);
			guiVol = StringConverter::parseReal(tLine[3]);
			soundVol = StringConverter::parseReal(tLine[4]);
			isShadowsOn = StringConverter::parseBool(tLine[5]);
			isBloom = StringConverter::parseBool(tLine[6]);
			if(tLine.size() >= 8)username = tLine[7];
			if(tLine.size() >= 9)generalCharname = StringConverter::parseBool(tLine[8]);
			if(tLine.size() >= 10)localUsername = StringConverter::parseBool(tLine[9]);
			if(tLine.size() >= 11)channelBlink = StringConverter::parseBool(tLine[10]);
			if(tLine.size() >= 12)runMode = StringConverter::parseBool(tLine[11]);
			if(tLine.size() >= 13)pagedGeometryOn = StringConverter::parseBool(tLine[12]);
			if(tLine.size() >= 14)viewDistance = StringConverter::parseReal(tLine[13]);
			if(tLine.size() >= 15)doubleJumpOn = StringConverter::parseBool(tLine[14]);
			if(tLine.size() >= 16)autoLipSync = StringConverter::parseBool(tLine[15]);
		}

		std::ifstream inFile2;
		inFile2.open("useWindowsCursor.dat", std::ifstream::in);
		if(inFile2.good())useWindowsCursor = true;
		inFile2.close();
	}

	// this need to be refactored toward a binary file or xml/ini/json
	void saveSettings(bool isTablet, const Real& mouseSensitivity, const Real& musicVol, const Real& guiVol, const Real& soundVol, const bool& isShadowsOn, bool isBloom, const String& username = "", bool generalCharname = false, bool localUsername = false, bool channelBlink = true, bool runMode = true, bool doubleJumpOn = true, bool autoLipSync = false, bool useWindowsCursor = false)
	{
		if(pagedGeometryForced)
		{
			pagedGeometryForced = false;
			pagedGeometryOn = false;
		}
		std::ofstream outFile;
		outFile.open("Settings.cfg", std::ofstream::out);

		String tBuffer = StringConverter::toString(isTablet) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(mouseSensitivity) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(musicVol) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(guiVol) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(soundVol) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(isShadowsOn) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(isBloom) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = username + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(generalCharname) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(localUsername) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(channelBlink) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(runMode) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(pagedGeometryOn) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(viewDistance) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(doubleJumpOn) + "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		tBuffer = StringConverter::toString(autoLipSync) + "\n#";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		outFile.close();

		if(useWindowsCursor)
		{
			std::ofstream outFile2;
			outFile2.open("useWindowsCursor.dat", std::ofstream::out);
			tBuffer = "1";
			outFile2.write(tBuffer.c_str(), (int)tBuffer.length());
			outFile2.close();
		}
		else _unlink("useWindowsCursor.dat");
	}
	void savePassword(bool flag, const String& password = "")
	{
		if(flag)
		{
			std::ofstream outFile;
			outFile.open("Settings2.dat", std::ios_base::binary);
			const String tBuffer = mMagixUtils.XOR7(password) + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
			outFile.close();
		}
		else _unlink("Settings2.dat");
	}
	const String loadPassword()
	{
		String tPassword = "";
		std::ifstream inFile;
		inFile.open("Settings2.dat", std::ios_base::binary);
		if(inFile.good())
		{
			char tBuffer[16] = "";
			inFile.getline(tBuffer, 16);
			tPassword = tBuffer;
		}
		inFile.close();
		return mMagixUtils.XOR7(tPassword);
	}
	void saveBanFile(bool flag, const unsigned short& numDays = 0)
	{
		if(flag)
		{
			String tDate = "Infinity";
			if(numDays != 0)
			{
				time_t rawtime;
				tm timeinfo;
				time(&rawtime);
				localtime_s(&timeinfo, &rawtime);
				int tYear = timeinfo.tm_year;
				int tDay = timeinfo.tm_yday + numDays;
				while (tDay > 365)
				{
					tYear += 1;
					tDay -= 365;
				}
				tDate = StringConverter::toString(tYear) + ";";
				tDate += StringConverter::toString(tDay);
			}

			std::ofstream outFile;
			outFile.open("Settings3.dat", std::ios_base::binary);
			String tBuffer = mMagixUtils.XOR7("Bannage") + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
			tBuffer = mMagixUtils.XOR7(tDate) + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
			outFile.close();
		}
		else _unlink("Settings3.dat");
	}
	const bool loadBanFile(unsigned short& numDays)
	{
		String tPassword = "";
		String tDate = "";
		std::ifstream inFile;
		inFile.open("Settings3.dat", std::ios_base::binary);
		if(inFile.good())
		{
			char tBuffer[16] = "";
			inFile.getline(tBuffer, 16);
			tPassword = tBuffer;
			tPassword = mMagixUtils.XOR7(tPassword);
			strcpy_s(tBuffer, 16, "");
			inFile.getline(tBuffer, 16);
			tDate = tBuffer;
			tDate = mMagixUtils.XOR7(tDate);
		}
		inFile.close();
		if(tPassword != "Bannage")return false;
		if(tDate == "Infinity")numDays = 0;
		else
		{
			int tYear = 0, tDay = 0;
			const vector<String>::type tPart = StringUtil::split(tDate, ";", 1);
			if(tPart.size() > 0)tYear = StringConverter::parseInt(tPart[0]);
			if(tPart.size() > 1)tDay = StringConverter::parseInt(tPart[1]);

			time_t rawtime;
			tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			tYear -= timeinfo.tm_year;
			tDay -= timeinfo.tm_yday;
			if(tYear < 0 || tYear == 0 && tDay <= 0)
			{
				_unlink("Settings3.dat");
				return false;
			}
			numDays = tYear * 365 + tDay;
		}
		return true;
	}
	
	

	const String loadWorldObjects(const String& filename)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(inFile.good())
		{
			inFile.seekg(0, std::ios::end);
			tSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);
			tBuffer = new char[tSize];
			strcpy(tBuffer, "");
			inFile.getline(tBuffer, tSize, '#');
			inFile.close();
			tData = tBuffer;
			delete[] tBuffer;
		}

		return tData;
	}

	// to be moved to campaign then ? is really called in MagixGUI this loads an array ?
	void loadCampaignList(vector<String>::type& list, bool customCampaigns = false)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(customCampaigns ? "CustomCampaigns.cfg" : "Campaigns.cfg", std::ifstream::in);
		if(!inFile.good())
		{
			if(!customCampaigns)loadCampaignList(list, true);
			return;
		}
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, '#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String>::type tCampaign = StringUtil::split(tData, "[#");
		for (int i = 0; i<int(tCampaign.size()); i++)
		{
			const vector<String>::type tLine = StringUtil::split(tCampaign[i], "\n", 2);
			if(tLine.size() > 0)
			{
				String tName = tLine[0];
				tName.erase(tName.find_first_of("]"), 1);
				list.push_back(tName);
			}
		}
		if(!customCampaigns)loadCampaignList(list, true);
	}

	// this could be an util but ... do we want it here anyway ? if the top one can be magixgui called
	// anyway this need to be more accurate : LoadCampaignConfig
	bool loadCampaign(const String& name, CampaignEventList& data, String& fileName, bool customCampaigns = false)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(customCampaigns ? "CustomCampaigns.cfg" : "Campaigns.cfg", std::ifstream::in);

		if(!inFile.good())
		{
			return (customCampaigns ? false : loadCampaign(name, data, fileName, true));
		}

		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, '#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String>::type tCampaign = StringUtil::split(tData, "[#");
		for (int i = 0; i<int(tCampaign.size()); i++)
		{
			const vector<String>::type tLine = StringUtil::split(tCampaign[i], "\n", 2);
			if(tLine.size() > 0)
			{
				String tName = tLine[0];
				tName.erase(tName.find_first_of("]"), 1);
				if(tName == name)
				{
					fileName = tLine[1];
					const vector<String>::type tEvent;
					loadCampaignScript(tLine[1], tEvent, data, 0, true);
					return true;
				}
			}
		}
		return (customCampaigns ? false : loadCampaign(name, data, fileName, true));
	}

	// clearly this one isnt a cfg reader
	void loadCampaignScript(const String& filename, const vector<String>::type& nextEvent, CampaignEventList& data, const unsigned short& eventCount = 0, bool loadFirstSection = false)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, char(128));
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String>::type tSection = StringUtil::split(tData, "[");

		if(nextEvent.size() > 0)
			for (int i = 0; i < (int)nextEvent.size(); i++)
			{
				const String tCurrentEvent = nextEvent[i];
				CampaignEvent tEvent;
				for (int j = 0; j < (int)tSection.size(); j++)
				{
					const vector<String>::type tLine = StringUtil::split(tSection[j], "\n");
					if(tLine.size() <= 0)continue;
					String tNextEvent = tLine[0];
					tNextEvent.erase(tNextEvent.find_first_of("]"), 1);
					if(tNextEvent == tCurrentEvent)
					{
						for (int k = 1; k < (int)tLine.size(); k++)
						{
							const vector<String>::type tPart = StringUtil::split(tLine[k], "=", 1);
							if(tPart.size() > 0)
							{
								const std::pair<String, String> tEventLine(tPart[0], tPart.size() > 1 ? tPart[1] : "");
								tEvent.push_back(tEventLine);
							}
						}
						break;
					}
				}
				data.push_back(tEvent);
			}
		else if(eventCount > 0 || loadFirstSection)
		{
			unsigned short tCounter = 0;
			CampaignEvent tEvent;
			for (int j = 0; j < (int)tSection.size(); j++)
			{
				if(tCounter == eventCount || loadFirstSection)
				{
					const vector<String>::type tLine = StringUtil::split(tSection[j], "\n");
					for (int k = 1; k < (int)tLine.size(); k++)
					{
						const vector<String>::type tPart = StringUtil::split(tLine[k], "=", 1);
						if(tPart.size() > 0)
						{
							const std::pair<String, String> tEventLine(tPart[0], tPart.size() > 1 ? tPart[1] : "");
							tEvent.push_back(tEventLine);
						}
					}
					break;
				}
				tCounter++;
			}
			data.push_back(tEvent);
		}
	}

	// to campaign
	bool loadCredits(const String& filename, vector<String>::type& credits)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(!inFile.good())return false;
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, '#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		credits.clear();

		const vector<String>::type tLine = StringUtil::split(tData, ";#");
		for (int i = 0; i<int(tLine.size()); i++)
		{
			if(tLine[i].length() > 0)
			{
				if(tLine[i][0] == '\n')
				{
					String tBuffer = tLine[i];
					tBuffer.erase(0, 1);
					credits.push_back(tBuffer);
				}
				else credits.push_back(tLine[i]);
			}
		}


		return true;
	}

	// for once code that is for the utilities/logic (game logic here)
	bool loadSavePoint(String& mapName, Vector2& point, unsigned char& dimension)
	{
		return loadHomePoint(mapName, point, dimension, true);
	}
	bool loadHomePoint(String& mapName, Vector2& point, unsigned char& dimension, bool isSavePoint = false)
	{
		std::ifstream inFile;
		inFile.open((isSavePoint ? "savepoint.sav" : "homepoint.sav"), std::ifstream::in);

		if(!inFile.good())
		{
			inFile.close();
			return false;
		}
		char tBuffer[32] = "";
		String tData;

		//Load map name
		inFile.getline(tBuffer, 32);
		tData = tBuffer;
		decrypt(tData);
		mapName = tData;

		if(!inFile.good())
		{
			inFile.close();
			return false;
		}
		strcpy(tBuffer, "");

		//Load map x
		inFile.getline(tBuffer, 32);
		tData = tBuffer;
		decrypt(tData);
		point.x = StringConverter::parseReal(tData);

		if(!inFile.good())
		{
			inFile.close();
			return false;
		}
		strcpy(tBuffer, "");

		//Load map z
		inFile.getline(tBuffer, 32);
		tData = tBuffer;
		decrypt(tData);
		point.y = StringConverter::parseReal(tData);

		if(!inFile.good())
		{
			inFile.close();
			dimension = 0;
			return true;
		}
		strcpy(tBuffer, "");

		//Load dimension
		inFile.getline(tBuffer, 32);
		tData = tBuffer;
		decrypt(tData);
		dimension = StringConverter::parseInt(tData);

		inFile.close();
		return true;
	}
	void saveSavePoint(const String& mapName, const Vector2& point, const unsigned char& dimension)
	{
		saveHomePoint(mapName, point, dimension, true);
	}
	void saveHomePoint(const String& mapName, const Vector2& point, const unsigned char& dimension, bool isSavePoint = false)
	{
		std::ofstream outFile;
		outFile.open((isSavePoint ? "savepoint.sav" : "homepoint.sav"), std::ofstream::out);

		//Save map name
		String tBuffer = mapName;
		encrypt(tBuffer);
		tBuffer += "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		//Save map x
		tBuffer = StringConverter::toString(point.x);
		encrypt(tBuffer);
		tBuffer += "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		//Save map z
		tBuffer = StringConverter::toString(point.y);
		encrypt(tBuffer);
		tBuffer += "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		//Save dimension
		tBuffer = StringConverter::toString(dimension);
		encrypt(tBuffer);
		tBuffer += "\n";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		outFile.close();
	}

	// TO GAME LOGIC !
	const vector<String>::type loadFriendList(bool isBlockList = false)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open((isBlockList ? "block.list" : "friend.list"), std::ifstream::in);
		if(!inFile.good())
		{
			vector<String>::type tBlank;
			tBlank.clear();
			return tBlank;
		}
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, ';');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		if(tData == "")
		{
			vector<String>::type tBlank;
			tBlank.clear();
			return tBlank;
		}

		return StringUtil::split(tData, "\n;");
	}
	const vector<String>::type loadBlockedList()
	{
		return loadFriendList(true);
	}

	void editFriendList(const String& name, bool add, bool isBlockList = false)
	{
		vector<String>::type tFriendList = loadFriendList(isBlockList);

		//File doesn't exist
		if(tFriendList.size() == 0)
		{
			if(add)
			{
				std::ofstream outFile;
				outFile.open((isBlockList ? "block.list" : "friend.list"), std::ofstream::out);
				const String tBuffer = name + "\n;";
				outFile.write(tBuffer.c_str(), (int)tBuffer.length());
				outFile.close();
			}
			return;
		}

		//Find existing name
		for (vector<String>::type::iterator it = tFriendList.begin(); it != tFriendList.end(); it++)
		{
			if(*it == name)
			{
				//Name already exists
				if(add)return;
				//Delete name
				tFriendList.erase(it);
				break;
			}
		}

		//Rewrite file
		std::ofstream outFile;
		outFile.open((isBlockList ? "block.list" : "friend.list"), std::ofstream::out);

		for (vector<String>::type::iterator it = tFriendList.begin(); it != tFriendList.end(); it++)
		{
			const String tBuffer = *it + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		}
		if(add)
		{
			const String tBuffer = name + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		}

		const String tBuffer = ";";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		outFile.close();
	}
	void editBlockedList(const String& name, bool add)
	{
		editFriendList(name, add, true);
	}
	bool isInFriendList(String name, bool isBlockList = false)
	{
		StringUtil::toLowerCase(name);
		const vector<String>::type tFriendList = loadFriendList(isBlockList);
		for (int i = 0; i < (int)tFriendList.size(); i++)
		{
			String tName = tFriendList[i];
			StringUtil::toLowerCase(tName);
			if(tName == name)return true;
		}
		return false;
	}
	bool isInBlockedList(String name)
	{
		return isInFriendList(name, true);
	}
	void saveBio(const String& name, const String& bio)
	{
		std::ofstream outFile;
		const String tFilename = name + ".bio";
		outFile.open(tFilename.c_str(), std::ofstream::out);

		const String tBuffer = bio + char(128);
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		outFile.close();
	}
	const String loadBio(const String& name = "")
	{
		if(name != "")lastBioName = name;
		if(lastBioName == "")return "";

		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		const String tFilename = lastBioName + ".bio";
		inFile.open(tFilename.c_str(), std::ifstream::in);
		if(!inFile.good())return "";
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, char(128));
		inFile.close();

		//Replace illegal characters
		for (int i = 0; i < tSize; i++)
		{
			if(tBuffer[i] == '\0')break;
			if((tBuffer[i] > char(126) || tBuffer[i]<char(32))
				&& tBuffer[i] != '\t' && tBuffer[i] != '\n')tBuffer[i] = '~';
		}

		tData = tBuffer;
		delete[] tBuffer;

		return tData;
	}
	/// <summary>
	/// end of logic stuf to be separated
	/// </summary>
	/// <returns></returns>
	
	const String loadUpdateCaption()
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open("update.txt", std::ifstream::in);
		if(!inFile.good())return "";
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, char(128));
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		return tData;
	}

	// items functions should be magix object 
	void loadItems(const String& filename, const bool decrypt)
	{
		maxItems = 0;
		itemMesh.clear();
		itemBone.clear();
		itemName.clear();
		itemIsPrivate.clear();
		itemHasOffset.clear();
		itemHasAltAnim.clear();
		itemParticle.clear();
		itemParticleOffset.clear();
		itemParticleOnNode.clear();

		if(decrypt)
		{
			vector<String>::type stream = mMagixUtils.XORInternal(filename);
			String line;

			for (int i = 0; i < (int)stream.size(); i++)
			{
				line = stream[i];

				StringUtil::trim(line);
				const vector<String>::type tPart = StringUtil::split(line, ";", 9);

				if(tPart.size() < 3)continue;
				if(tPart.size() > 0)itemMesh.push_back(tPart[0]);
				if(tPart.size() > 1)itemName.push_back(tPart[1]);
				if(tPart.size() > 2)itemBone.push_back(tPart[2]);
				if(tPart.size() > 3)itemIsPrivate.push_back(tPart[3] == "1" ? true : false);
				if(tPart.size() > 4)itemHasOffset.push_back(tPart[4] == "1" ? true : false);
				else itemHasOffset.push_back(false);
				if(tPart.size() > 5)itemHasAltAnim.push_back(tPart[5] == "1" ? true : false);
				else itemHasAltAnim.push_back(false);
				if(tPart.size() > 6)itemParticle.push_back(tPart[6]);
				else itemParticle.push_back("");
				if(tPart.size() > 7)itemParticleOffset.push_back(tPart[7]);
				else itemParticleOffset.push_back("");
				if(tPart.size() > 8)itemParticleOnNode.push_back(tPart[8] == "1" ? true : false);
				else itemParticleOnNode.push_back(false);
			}
			maxItems = int(itemMesh.size());
		}
		else
		{
			long tSize = 0;
			char* tBuffer;
			String tData = "";

			std::ifstream inFile;
			inFile.open(filename.c_str(), std::ifstream::in);
			if(!inFile.good())
			{
				Ogre::LogManager::getSingleton().logMessage("IT INITIALIZE ERROR: Unable to open file " + filename);
				return;
			}
			inFile.seekg(0, std::ios::end);
			tSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);
			tBuffer = new char[tSize];
			strcpy(tBuffer, "");
			inFile.getline(tBuffer, tSize, '#');
			inFile.close();
			tData = tBuffer;
			delete[] tBuffer;

			const vector<String>::type tLine = StringUtil::split(tData, "\n#");

			maxItems = int(tLine.size());
			for (int i = 0; i < maxItems; i++)
			{
				const vector<String>::type tPart = StringUtil::split(tLine[i], ";", 9);

				if(tPart.size() > 0)itemMesh.push_back(tPart[0]);
				if(tPart.size() > 1)itemName.push_back(tPart[1]);
				if(tPart.size() > 2)itemBone.push_back(tPart[2]);
				if(tPart.size() > 3)itemIsPrivate.push_back(tPart[3] == "1" ? true : false);
				if(tPart.size() > 4)itemHasOffset.push_back(tPart[4] == "1" ? true : false);
				else itemHasOffset.push_back(false);
				if(tPart.size() > 5)itemHasAltAnim.push_back(tPart[5] == "1" ? true : false);
				else itemHasAltAnim.push_back(false);
				if(tPart.size() > 6)itemParticle.push_back(tPart[6]);
				else itemParticle.push_back("");
				if(tPart.size() > 7)itemParticleOffset.push_back(tPart[7]);
				else itemParticleOffset.push_back("");
				if(tPart.size() > 8)itemParticleOnNode.push_back(tPart[8] == "1" ? true : false);
				else itemParticleOnNode.push_back(false);
			}
		}
	}
	const String getItemName(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
			if(itemMesh[i] == meshName && (int)itemName.size() > i)return itemName[i];
		return "";
	}
	const String getItemBone(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
			if(itemMesh[i] == meshName && (int)itemBone.size() > i)return itemBone[i];
		return "";
	}
	const bool getItemIsPrivate(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
			if(itemMesh[i] == meshName && (int)itemIsPrivate.size() > i)return itemIsPrivate[i];
		return false;
	}
	const bool getItemHasOffset(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
			if(itemMesh[i] == meshName && (int)itemHasOffset.size() > i)return itemHasOffset[i];
		return false;
	}

	const bool getItemHasAltAnim(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
		{
			if (itemMesh[i] == meshName && (int)itemHasAltAnim.size() > i)
			{
				return itemHasAltAnim[i];
			}
		}

		return false;
	}

	const String getItemParticle(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
		{
			if (itemMesh[i] == meshName && (int)itemParticle.size() > i)
			{
				return itemParticle[i];
			}
		}

		return "";
	}

	const std::pair<Vector3, bool> getItemParticleOffset(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
			if(itemMesh[i] == meshName && (int)itemParticleOffset.size() > i)
			{
				if((int)itemParticleOnNode.size() > i)return std::pair<Vector3, bool>(StringConverter::parseVector3(itemParticleOffset[i]), itemParticleOnNode[i]);
				else return std::pair<Vector3, bool>(StringConverter::parseVector3(itemParticleOffset[i]), false);
			}
		return std::pair<Vector3, bool>(Vector3::ZERO, false);
	}
	const bool itemMeshExists(const String& meshName)
	{
		for (int i = 0; i < maxItems; i++)
			if(itemMesh[i] == meshName)return true;
		return false;
	}
	
	
	void loadHotkeys(const String& filename)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(inFile.good())
		{
			inFile.seekg(0, std::ios::end);
			tSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);
			tBuffer = new char[tSize];
			strcpy(tBuffer, "");
			inFile.getline(tBuffer, tSize, '#');
			tData = tBuffer;
			delete[] tBuffer;
		}
		inFile.close();

		const vector<String>::type tLine = StringUtil::split(tData, "\n#");
		if((int)tLine.size() == MAX_HOTKEYF)
			for (int i = 0; i < MAX_HOTKEYF; i++)hotkeyF[i] = tLine[i];
		else
		{
			//Default values
			hotkeyF[0] = "Normal";
			hotkeyF[1] = "Grin";
			hotkeyF[2] = "EvilGrin";
			hotkeyF[3] = "Frown";
			hotkeyF[4] = "Shock";
			hotkeyF[5] = "Smirk";
			hotkeyF[6] = "Rest";
			hotkeyF[7] = "Tongue";
		}
	}

	// so this is more and more like a MasterSettings than External Definitions
	const void saveHotkeys(const String& filename)
	{
		std::ofstream outFile;
		outFile.open(filename.c_str(), std::ofstream::out);

		for (int i = 0; i < MAX_HOTKEYF; i++)
		{
			const String tBuffer = hotkeyF[i] + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		}
		const String tBuffer = "#";
		outFile.write(tBuffer.c_str(), (int)tBuffer.length());

		outFile.close();
	}
	void setHotkeyF(const int& key, const String& value)
	{
		if(key >= 0 && key < MAX_HOTKEYF)hotkeyF[key] = value;
	}
	const String getHotkeyF(const int& key)
	{
		if(key >= 0 && key < MAX_HOTKEYF)return hotkeyF[key];
		return "";
	}

	// to game logic BUT an "utils" function ? IF used outside of chargen ? :D
	bool hasManeMesh(const String& mesh)
	{
		for (int i = 0; i < (int)maneMesh.size(); i++)
		{
			if (maneMesh[i] == mesh)
			{
				return true;
			}
		}
		return false;
	}

	bool hasTailMesh(const String& mesh)
	{
		for (int i = 0; i < (int)tailMesh.size(); i++)
		{
			if (tailMesh[i] == mesh)
			{
				return true;
			}
		}

		return false;
	}
	bool hasWingMesh(const String& mesh)
	{
		for (int i = 0; i < (int)wingMesh.size(); i++)
			if(wingMesh[i] == mesh)return true;
		return false;
	}
	bool hasTuftMesh(const String& mesh)
	{
		for (int i = 0; i < (int)tuftMesh.size(); i++)
			if(tuftMesh[i] == mesh)return true;
		return false;
	}
	const bool hasBodyMark(const unsigned char& id)
	{
		return (id < maxBodyMarks);
	}
	const bool hasHeadMark(const unsigned char& id)
	{
		return (id < maxHeadMarks);
	}
	const bool hasTailMark(const unsigned char& id)
	{
		return (id < maxTailMarks);
	}

	///
	/// The function unders are going to move they should not be there
	/// 

	// this is a .. maybe, and by maybe i mean a "customizable central folder" not separated everywhere
	void loadWeatherCycle(const String& type, vector<WeatherEvent>::type& list, bool isCustom = false)
	{
		String tFilename = "";
		list.clear();

		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(isCustom ? "media/weather/CustomWeatherCycles.cfg" : "media/weather/WeatherCycles.cfg", std::ifstream::in);
		if(!inFile.good())return;
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, '#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String>::type tPart = StringUtil::split(tData, "[#");
		for (int i = 0; i<int(tPart.size()); i++)
		{
			const vector<String>::type tLine = StringUtil::split(tPart[i], "\n", 2);
			if(tLine.size() > 1)
			{
				String tType = tLine[0];
				tType.erase(tType.find_first_of("]"), 1);
				if(tType == type)
				{
					tFilename = tLine[1];
					break;
				}
			}
		}

		if(tFilename == "" && !isCustom)
		{
			loadWeatherCycle(type, list, true);
			return;
		}

		ConfigFile cf;
		cf.load("media/weather/" + tFilename);
		
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		while (seci.hasMoreElements())
		{
			const String tSectionName = seci.peekNextKey();
			
			ConfigFile::SettingsMultiMap* settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			WeatherEvent tEvent;
			
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				if(StringUtil::startsWith(i->first, "start"))tEvent.start = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "end"))tEvent.end = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "skyshader"))tEvent.skyShader = StringConverter::parseColourValue(i->second);
				else if(StringUtil::startsWith(i->first, "skyadder"))tEvent.skyAdder = StringConverter::parseColourValue(i->second);
				else if(StringUtil::startsWith(i->first, "weather"))tEvent.type = i->second;
				else if(StringUtil::startsWith(i->first, "rate"))tEvent.rate = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "constant"))tEvent.isConstant = StringConverter::parseBool(i->second);
				else if(StringUtil::startsWith(i->first, "effectfrequency"))tEvent.effectFrequency = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "effect"))tEvent.effect.push_back(i->second);
			}

			if(tSectionName != "")
			{
				list.push_back(tEvent);
			}
		}
	}

	// by design weather should be separate and under same namespace as sky.. and the code simplifiedn why sometimes using ConfigFile and others a ifstream ?
	// also, we could have a single function that do it all and returns a buffer
	void loadWeather(const String& type, String& particle, Vector3& offset, String& sound, bool isCustom = false)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(isCustom ? "media/weather/CustomWeathers.cfg" : "media/weather/Weathers.cfg", std::ifstream::in);
		if(!inFile.good())return;
		inFile.seekg(0, std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer, "");
		inFile.getline(tBuffer, tSize, '#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String>::type tPart = StringUtil::split(tData, "[#");
		for (int i = 0; i<int(tPart.size()); i++)
		{
			const vector<String>::type tLine = StringUtil::split(tPart[i], "\n", 4);
			if(tLine.size() >= 2)
			{
				if(StringUtil::startsWith(tLine[0], type, false))
				{
					particle = tLine[1];
					if(tLine.size() >= 3)
					{
						offset = StringConverter::parseVector3(tLine[2]);
					}
					if(tLine.size() >= 4)
					{
						sound = tLine[3];
					}

					return;
				}
			}
		}
		if(!isCustom)loadWeather(type, particle, offset, sound, true);
	}

	// again, gameplay stuff but this one is settings
	void loadAttacks(const String& filename, bool isCustom)
	{
		String tFilename = filename;
		if(isCustom)
		{
			std::ifstream inFile(filename.c_str());
			if(!inFile.good())return;
			else inFile.close();
		}
		//Genereate temp file
		else
		{
			tFilename = filename + ".cfg";
			if(!mMagixUtils.XOR7FileGen(filename, tFilename, true, true))
				throw(Exception(9, "Corrupted Data File", filename + ", please run the autopatcher."));
		}
		ConfigFile cf;
		cf.load(tFilename);

		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		while (seci.hasMoreElements())
		{
			const String tSectionName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap* settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;

			// our anim & fx config
			Attack tAtk;

			for (i = settings->begin(); i != settings->end(); ++i)
			{
				// get the trailFX_ 
				if(StringUtil::startsWith(i->first, "trailfx"))
				{
					// split value character is comma the ","
					const vector<String>::type tPart = StringUtil::split(i->second, ",", 1);

					if(tPart.size() == 2)
					{
						// whats the tPart actually ? need to walk on that, breakpoint.
						// educated guess : everything that is within trailfx ?
						const unsigned short tFxID = StringConverter::parseInt(tPart[0]) - 1;

						while ((int)tAtk.FX.size() < (tFxID + 1))
						{
							tAtk.FX.push_back(AttackFX());
						}
						
						if(StringUtil::endsWith(i->first, "bone"))
						{
							tAtk.FX[tFxID].bone = tPart[1];
						}
						else if(StringUtil::endsWith(i->first, "trailmat"))
						{
							tAtk.FX[tFxID].trailMat = tPart[1];
						}
						else if(StringUtil::endsWith(i->first, "width"))
						{
							tAtk.FX[tFxID].trailWidth = StringConverter::parseReal(tPart[1]);
						}
						else if(StringUtil::endsWith(i->first, "colour"))
						{
							tAtk.FX[tFxID].colour = StringConverter::parseColourValue(tPart[1]);
						}
						else if(StringUtil::endsWith(i->first, "colourchange"))
						{
							tAtk.FX[tFxID].colourChange = StringConverter::parseColourValue(tPart[1]);
						}
						else if(StringUtil::endsWith(i->first, "trailheadmat"))
						{
							tAtk.FX[tFxID].trailHeadMat = tPart[1];
						}
					}
				}
				// weird its parsed here while its first parameter after name
				else if(StringUtil::startsWith(i->first, "anim"))
				{
					tAtk.anim = i->second;
				}
				else if(StringUtil::startsWith(i->first, "range"))
				{
					tAtk.range = StringConverter::parseReal(i->second);
				}
				else if(StringUtil::startsWith(i->first, "hitforce"))
				{
					tAtk.hitForce = StringConverter::parseVector3(i->second);
				}
				else if(StringUtil::startsWith(i->first, "offset"))
				{
					tAtk.offset = StringConverter::parseVector3(i->second);
				}
				else if(StringUtil::startsWith(i->first, "hpmin"))
				{
					tAtk.hpMin = StringConverter::parseReal(i->second);
				}
				else if(StringUtil::startsWith(i->first, "hpmax"))
				{
					tAtk.hpMax = StringConverter::parseReal(i->second);
				}
				else if(StringUtil::startsWith(i->first, "hitally"))
				{
					tAtk.hitAlly = StringConverter::parseBool(i->second);
				}
				else if(StringUtil::startsWith(i->first, "autotarget"))
				{
					tAtk.autoTarget = StringConverter::parseBool(i->second);
				}
				else if(StringUtil::startsWith(i->first, "singletarget"))
				{
					tAtk.singleTarget = StringConverter::parseBool(i->second);
				}
				else if(StringUtil::startsWith(i->first, "moveforce"))
				{
					tAtk.moveForce = StringConverter::parseVector3(i->second);
				}
				else if(StringUtil::startsWith(i->first, "speedmultiplier"))
				{
					tAtk.speedMultiplier = StringConverter::parseReal(i->second);
				}
				else if(StringUtil::startsWith(i->first, "soundroar"))
				{
					tAtk.soundRoar = StringConverter::parseBool(i->second);
				}
				else if(StringUtil::startsWith(i->first, "attackrange"))
				{
					tAtk.attackRange = StringConverter::parseInt(i->second);
				}
			}
			if(tSectionName != "")
			{
				tAtk.name = tSectionName;
				attackList.push_back(tAtk);
			}
		}
		//Delete temp file
		if(!isCustom)
		{
			_unlink(tFilename.c_str());
		}
	}

	// unit manager & mgui use this (unit manager is player manager ??)
	// use an array of attack that was built in loadattacks

	// unit manager & mgui use this (unit manager is player manager ??)
	const Attack getAttack(const String& name)
	{
		for (int i = 0; i < (int)attackList.size(); i++)
		{
			if(attackList[i].name == name)
			{
				return attackList[i];
			}
		}
		return Attack();
	}

	// unit & player
	const String getAttackAnim(const String& name)
	{
		for (int i = 0; i < (int)attackList.size(); i++)
		{
			if(attackList[i].name == name)
			{
				return attackList[i].anim;
			}
		}

		return "Attack1";
	}

	// is really for unity manager ?
	bool isSkillTargetsAllies(const String& name)
	{
		for (int i = 0; i < (int)attackList.size(); i++)
		{
			if(attackList[i].name == name)
			{
				return attackList[i].hitAlly;
			}
		}

		return false;
	}

	bool isSkillTargetsSelf(const String& name)
	{
		for (int i = 0; i < (int)attackList.size(); i++)
		{
			if(attackList[i].name == name)
			{
				return attackList[i].hitAlly;
			}
		}

		return false;
	}

	// wip notes : this is gameplay, BUT cd1.dat so this can stay but maybe reworked name etc
	void loadCritters(const String &filename, bool isCustom)
	{
		String tFilename = filename;
		if(isCustom)
		{
			std::ifstream inFile(filename.c_str());
			if(!inFile.good())return;
			else inFile.close();
		}
		// temp file.. ok but what ? do this uncipher .dat ?
		else
		{
			tFilename = filename + ".cfg";
			if(!mMagixUtils.XOR7FileGen(filename, tFilename, true, true))
				throw(Exception(9, "XOR7FileGen unable to run. Corrupted Data File ?", filename + ", re-run the IT XOR tool if you are sure of your file."));
		}
		ConfigFile cf;
		cf.load(tFilename);
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		while (seci.hasMoreElements())
		{
			const String tSectionName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap* settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			Critter tC;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				if(StringUtil::startsWith(i->first, "mesh"))tC.mesh = i->second;
				else if(StringUtil::startsWith(i->first, "isuncustomizable"))
				{
					if(!isCustom)tC.isUncustomizable = StringConverter::parseBool(i->second);
				}
				else if(StringUtil::startsWith(i->first, "isdrawpoint"))
				{
					if(!isCustom)tC.isDrawPoint = StringConverter::parseBool(i->second);
				}
				else if(StringUtil::startsWith(i->first, "material"))tC.material = i->second;
				else if(StringUtil::startsWith(i->first, "sound"))tC.sound = i->second;
				else if(StringUtil::startsWith(i->first, "friendly"))tC.friendly = StringConverter::parseBool(i->second);
				else if(StringUtil::startsWith(i->first, "invulnerable"))tC.invulnerable = StringConverter::parseBool(i->second);
				else if(StringUtil::startsWith(i->first, "scale"))tC.scale = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "hp"))tC.hp = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "flying"))tC.flying = StringConverter::parseBool(i->second);
				else if(StringUtil::startsWith(i->first, "decisionmin"))tC.decisionMin = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "decisiondeviation"))tC.decisionDeviation = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "maxspeed"))tC.maxSpeed = StringConverter::parseReal(i->second);
				else if(StringUtil::startsWith(i->first, "droplist"))
				{
					const vector<String>::type tPart = StringUtil::split(i->second, ",", 1);
					if(tPart.size() >= 2)tC.dropList.push_back(std::pair<String, Real>(tPart[0], StringConverter::parseReal(tPart[1])));
				}
				else if(StringUtil::startsWith(i->first, "skilldrop"))
				{
					const vector<String>::type tPart = StringUtil::split(i->second, ",", 1);
					if(tPart.size() >= 2)tC.skillDrop = std::pair<String, unsigned char>(tPart[0], StringConverter::parseInt(tPart[1]));
				}
				else if(StringUtil::startsWith(i->first, "attacklist"))
				{
					const vector<String>::type tPart = StringUtil::split(i->second, ",", 5);
					if(tPart.size() >= 2)tC.attackList.push_back(CritterAttack(StringConverter::parseReal(tPart[0]),
						StringConverter::parseReal(tPart[1]),
						(tPart.size() >= 5 ? Vector3(StringConverter::parseReal(tPart[2]), StringConverter::parseReal(tPart[3]), StringConverter::parseReal(tPart[4])) : Vector3(0, 0, 1)),
						(tPart.size() >= 6 ? StringConverter::parseBool(tPart[5]) : false)
					));
				}
			}
			if(tSectionName != "")
			{
				tC.type = tSectionName;
				critterList.push_back(tC);
			}
		}

		//Delete temp file
		if(!isCustom)
		{
			_unlink(tFilename.c_str());
		}
	}
	const Critter getCritter(const String& type)
	{
		for (int i = 0; i < (int)critterList.size(); i++)
		{
			if(critterList[i].type == type)return critterList[i];
		}
		return Critter();
	}
	const vector<std::pair<String, Real>>::type getCritterDropList(const String& type)
	{
		for (int i = 0; i < (int)critterList.size(); i++)
		{
			if(critterList[i].type == type)return critterList[i].dropList;
		}
		const vector<std::pair<String, Real>>::type tList;
		return tList;
	}
	const std::pair<String, unsigned char> getCritterSkillDrop(const String& type)
	{
		for (int i = 0; i < (int)critterList.size(); i++)
		{
			if(critterList[i].type == type)return critterList[i].skillDrop;
		}
		return std::pair<String, unsigned char>("", 0);
	}

	const std::pair<String, unsigned char> getRandomSkillDrop()
	{
		const unsigned short MAX_RANDOMSKILL = 8;
		unsigned short tID = (unsigned short)Math::RangeRandom(0, MAX_RANDOMSKILL);
		
		if(tID >= MAX_RANDOMSKILL)
		{
			tID = MAX_RANDOMSKILL - 1;
		}

		switch (tID)
		{
			case 0:
				return std::pair<String, unsigned char>("Cure", 10);
			case 1: 
				return std::pair<String, unsigned char>("Lunge", 10);
			case 2: 
				return std::pair<String, unsigned char>("Sweep", 10);
			case 3: 
				return std::pair<String, unsigned char>("Stomp", 10);
			case 4: 
				return std::pair<String, unsigned char>("Backflip", 10);
			case 5: 
				return std::pair<String, unsigned char>("Tame", 3);
			case 6: 
				return std::pair<String, unsigned char>("Roar", 6);
			case 7: 
				return std::pair<String, unsigned char>("Zoom", 10);
		}
		return std::pair<String, unsigned char>("", 0);
	}
	const unsigned char getCritterRandomAttack(const String& type, bool* hitAlly = 0)
	{
		for (int i = 0; i < (int)critterList.size(); i++)
		{
			if(critterList[i].type == type)
			{
				if(critterList[i].attackList.size() == 0)
				{
					return 0;
				}
				
				//Attacks 2 and above are rarer
				if(critterList[i].attackList.size() == 1 || Math::UnitRandom() < 0.5)
				{
					if(hitAlly)*hitAlly = critterList[i].attackList[0].hitAlly;
					return 1;
				}
				
				unsigned char tID = Math::RangeRandom(1, (int)critterList[i].attackList.size());
				
				if(tID >= (int)critterList[i].attackList.size())
				{
					tID -= 1;
				}
				if(hitAlly)
				{
					*hitAlly = critterList[i].attackList[tID].hitAlly;
				}

				return tID + 1;
			}
		}
		return 0;
	}
	const unsigned char getCritterRandomSpecificAttack(const String& type, bool getNonHeal)
	{
		for (int i = 0; i < (int)critterList.size(); i++)
		{
			if(critterList[i].type == type)
			{
				vector<unsigned char>::type tList;

				for (int j = 0; j < (int)critterList[i].attackList.size(); j++)
				{
					if(getNonHeal ^ critterList[i].attackList[j].hitAlly)tList.push_back(j + 1);
				}
					
				if(tList.size() == 0)
				{
					return 0;
				}

				//Attacks 2 and above are rarer
				if(tList.size() == 1 || Math::UnitRandom() < 0.5)
				{
					return tList[0];
				}

				unsigned char tID = Math::RangeRandom(1, (int)tList.size());
				if(tID >= (int)tList.size())
				{
					tID -= 1;
				}

				return tList[tID];
			}
		}
		return 0;
	}

	const std::pair<CritterAttack, String> getCritterAttackAndSound(const String& type, const unsigned char& iID)
	{
		for (int i = 0; i < (int)critterList.size(); i++)
		{
			if(critterList[i].type == type)
			{
				if(iID >= 0 && iID < (int)critterList[i].attackList.size())
				{
					return std::pair<CritterAttack, String>(critterList[i].attackList[iID], critterList[i].sound);
				}
				else
				{
					return std::pair<CritterAttack, String>(CritterAttack(0, 0, Vector3::ZERO), critterList[i].sound);
				}
			}
		}
		return std::pair<CritterAttack, String>(CritterAttack(0, 0, Vector3::ZERO), "");
	}

	// settings
	bool loadCritterSpawnList(const String& worldName,
		unsigned short& limit,
		vector<WorldCritter>::type& list,
		vector<std::pair<Vector3, Vector3>>::type& roamArea,
		const String& customFilename = "")
	{
		//Hardcoded goodness
		if(worldName != "Default")
		{
			limit = 3;
			//Draw Points for crittered maps
			list.push_back(WorldCritter("Draw Point", 0.05));
		}
		else return true;
		if(loadCritterSpawnListFile("cd2.dat", worldName, limit, list, roamArea))return true;

		if(loadCustomCritterSpawnList("media/terrains/" + worldName + "/" + customFilename, limit, list, roamArea))return true;
		return false;
	}

	bool loadCritterSpawnListFile(const String& filename, const String& worldName, unsigned short& limit, vector<WorldCritter>::type& list, vector<std::pair<Vector3, Vector3>>::type& roamArea)
	{
		const String tFilename = filename + ".cfg";
		if(!mMagixUtils.XOR7FileGen(filename, tFilename, true, true))return false;
		ConfigFile cf;
		cf.load(tFilename);
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		while (seci.hasMoreElements())
		{
			const String tSectionName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap* settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;

			if(tSectionName == worldName)
			{
				for (i = settings->begin(); i != settings->end(); ++i)
				{
					if(StringUtil::startsWith(i->first, "limit"))limit = StringConverter::parseInt(i->second);
					else if(StringUtil::startsWith(i->first, "clearlist") && StringConverter::parseBool(i->second))list.clear();
					else if(StringUtil::startsWith(i->first, "critterlist"))
					{
						const vector<String>::type tPart = StringUtil::split(i->second, ",", 3);
						if(tPart.size() >= 2)list.push_back(WorldCritter(tPart[0],
							StringConverter::parseReal(tPart[1]),
							(tPart.size() >= 4 ? StringConverter::parseBool(tPart[2]) : false),
							(tPart.size() >= 4 ? StringConverter::parseInt(tPart[3]) : 0)
						));
					}
					else if(StringUtil::startsWith(i->first, "roamarea"))
					{
						const vector<String>::type tPart = StringUtil::split(i->second, ",", 5);
						if(tPart.size() >= 6)roamArea.push_back(std::pair<Vector3, Vector3>(Vector3(StringConverter::parseReal(tPart[0]), StringConverter::parseReal(tPart[1]), StringConverter::parseReal(tPart[2])),
							Vector3(StringConverter::parseReal(tPart[3]), StringConverter::parseReal(tPart[4]), StringConverter::parseReal(tPart[5]))
						));
					}
				}
				//Delete temp file
				_unlink(tFilename.c_str());
				return true;
			}
		}

		//Delete temp file
		_unlink(tFilename.c_str());
		return false;
	}

	bool loadCustomCritterSpawnList(const String& filename, unsigned short& limit, vector<WorldCritter>::type& list, vector<std::pair<Vector3, Vector3>>::type& roamArea)
	{
		long tSize = 0;
		char* tBuffer;
		String tData = "";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(inFile.good())
		{
			inFile.seekg(0, std::ios::end);
			tSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);
			tBuffer = new char[tSize];
			strcpy(tBuffer, "");
			inFile.getline(tBuffer, tSize, '#');
			inFile.close();
			tData = tBuffer;
			delete[] tBuffer;
		}
		else
		{
			inFile.close();
			return false;
		}

		limit = 0;
		list.clear();
		roamArea.clear();

		const vector<String>::type tSection = StringUtil::split(tData, "[#");
		for (int i = 0; i<int(tSection.size()); i++)
		{
			const vector<String>::type tLine = StringUtil::split(tSection[i], "\n");
			if(tLine.size() > 0)
			{
				if(StringUtil::startsWith(tLine[0], "Limit", false) && tLine.size() >= 2)
				{
					limit = StringConverter::parseInt(tLine[1]);
					if(limit > 200)limit = 200;
				}
				else if(StringUtil::startsWith(tLine[0], "Critter", false) && tLine.size() >= 3)
				{
					WorldCritter tC;
					for (int j = 1; j < (int)tLine.size(); j++)
					{
						const vector<String>::type tPart = StringUtil::split(tLine[j], "=");
						if(tPart.size() < 2)continue;
						if(StringUtil::startsWith(tPart[0], "type"))tC.type = tPart[1];
						else if(StringUtil::startsWith(tPart[0], "rate"))tC.spawnRate = StringConverter::parseReal(tPart[1]);
						else if(StringUtil::startsWith(tPart[0], "roamareaindex"))
						{
							tC.hasRoamArea = true;
							tC.roamAreaID = StringConverter::parseInt(tPart[1]);
						}
					}
					if(tC.type != "")
					{
						const Critter tCritter = getCritter(tC.type);
						if(tCritter.type != "" && !tCritter.isUncustomizable)list.push_back(tC);
					}
				}
				else if(StringUtil::startsWith(tLine[0], "roamarea") && tLine.size() >= 3)
				{
					std::pair<Vector3, Vector3> tArea(Vector3::ZERO, Vector3::ZERO);
					for (int j = 1; j < (int)tLine.size(); j++)
					{
						const vector<String>::type tPart = StringUtil::split(tLine[j], "=");
						if(tPart.size() < 2)continue;
						if(StringUtil::startsWith(tPart[0], "start"))tArea.first = StringConverter::parseVector3(tPart[1]);
						else if(StringUtil::startsWith(tPart[0], "range"))tArea.second = StringConverter::parseVector3(tPart[1]);
					}
					if(tArea.first != Vector3::ZERO || tArea.second != Vector3::ZERO)roamArea.push_back(tArea);
				}
			}
		}
		return true;
	}
	const String loadLocalIP(const String& filename)
	{
		String tIP = "";
		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(inFile.good())
		{
			char tBuffer[32];
			inFile.read(tBuffer, 32);
			tIP = tBuffer;
		}
		inFile.close();

		return tIP;
	}

};

#endif