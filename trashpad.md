trashpad 
code that was obsolete and removed, tracked for v1

---
from external defs in loadUnitMeshes
---
		/*maxHeads = 0;
		maxManes = 0;
		maxTails = 0;
		maxWings = 0;
		headMesh.clear();
		maneMesh.clear();
		tailMesh.clear();
		wingMesh.clear();

		long tSize = 0;
		char *tBuffer;
		String tData="";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		inFile.seekg(0,std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0,std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer,"");
		inFile.getline(tBuffer,tSize,'#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		const vector<String> tPart = StringUtil::split(tData,"[#",4);
		if(tPart.size()==4)
		for(int i=0;i<4;i++)
		{
			const vector<String> tMesh = StringUtil::split(tPart[i],"\n");

			if(tMesh[0]=="HeadMesh]")
			{
				maxHeads = int(tMesh.size())-1;
				for(int j=1;j<=maxHeads;j++)headMesh.push_back(tMesh[j]);
			}
			else if(tMesh[0]=="ManeMesh]")
			{
				maxManes = int(tMesh.size())-1;
				for(int j=1;j<=maxManes;j++)maneMesh.push_back(tMesh[j]);
			}
			else if(tMesh[0]=="TailMesh]")
			{
				maxTails = int(tMesh.size())-1;
				for(int j=1;j<=maxTails;j++)tailMesh.push_back(tMesh[j]);
			}
			else if(tMesh[0]=="WingMesh]")
			{
				maxWings = int(tMesh.size())-1;
				for(int j=1;j<=maxWings;j++)wingMesh.push_back(tMesh[j]);
			}
		}*/
---
from external defs in isRestricted
---
		/*long tSize = 0;
		char *tBuffer;
		String tData="";

		std::ifstream inFile;
		inFile.open("MeshRestrictions.cfg", std::ifstream::in);
		inFile.seekg(0,ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0,ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer,"");
		inFile.getline(tBuffer,tSize,'#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		vector<String> tPart = StringUtil::split(tData,"[#");
		if(tPart.size()>0)
		for(int i=0;i<int(tPart.size());i++)
		{
			vector<String> tLine = StringUtil::split(tPart[i],"\n");
			if(tLine.size()>0)
			{
				tLine[0].erase(tLine[0].find_first_of("]"));
				if(tLine[0]==headMesh[headID])
				for(int j=1;j<int(tLine.size());j++)
				{
					if(maneMesh[maneID]==tLine[j])return true;
				}
			}
		}
		return false;*/
---
in external defs old loadWorld
---
	/*bool loadWorld(const String &name, String &terrain, Real &x, Real &z, Vector3 &spawnSquare, String &grassMat, String &grassMap, String &grassColourMap, String &tree1, String &tree2, String &tree3, unsigned short &treeCount, String &bush1, String &bush2, String &bush3)
	{
		long tSize = 0;
		char *tBuffer;
		String tData="";

		std::ifstream inFile;
		inFile.open("Worlds.cfg", std::ifstream::in);
		inFile.seekg(0,std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0,std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer,"");
		inFile.getline(tBuffer,tSize,'#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		vector<String> tMap = StringUtil::split(tData,"[#");
		for(int i=0;i<int(tMap.size());i++)
		{
			vector<String> tLine = StringUtil::split(tMap[i],"\n",15);
			if(tLine.size()>0)
			{
				tLine[0].erase(tLine[0].find_first_of("]"));
				if(tLine[0]==name)
				{
					terrain = tLine[1];
					x = StringConverter::parseReal(tLine[2]);
					z = StringConverter::parseReal(tLine[3]);
					spawnSquare = StringConverter::parseVector3(tLine[4]);
					if(tLine.size()>=6)grassMat = tLine[5];
					if(tLine.size()>=7)grassMap = tLine[6];
					if(tLine.size()>=8)grassColourMap = tLine[7];
					if(tLine.size()>=9)tree1 = tLine[8];
					if(tLine.size()>=10)tree2 = tLine[9];
					if(tLine.size()>=11)tree3 = tLine[10];
					if(tLine.size()>=12)treeCount = StringConverter::parseInt(tLine[11]);
					if(tLine.size()>=13)bush1 = tLine[12];
					if(tLine.size()>=14)bush2 = tLine[13];
					if(tLine.size()>=15)bush3 = tLine[14];
					return true;
				}
			}
		}

		//Look in customWorlds.cfg
		inFile.open("CustomWorlds.cfg", std::ifstream::in);
		inFile.seekg(0,std::ios::end);
		tSize = inFile.tellg();
		inFile.seekg(0,std::ios::beg);
		tBuffer = new char[tSize];
		strcpy(tBuffer,"");
		inFile.getline(tBuffer,tSize,'#');
		inFile.close();
		tData = tBuffer;
		delete[] tBuffer;

		tMap = StringUtil::split(tData,"[#");
		for(int i=0;i<int(tMap.size());i++)
		{
			vector<String> tLine = StringUtil::split(tMap[i],"\n",15);
			if(tLine.size()>0)
			{
				tLine[0].erase(tLine[0].find_first_of("]"));
				if(tLine[0]==name)
				{
					terrain = tLine[1];
					x = StringConverter::parseReal(tLine[2]);
					z = StringConverter::parseReal(tLine[3]);
					spawnSquare = StringConverter::parseVector3(tLine[4]);
					if(tLine.size()>=6)grassMat = tLine[5];
					if(tLine.size()>=7)grassMap = tLine[6];
					if(tLine.size()>=8)grassColourMap = tLine[7];
					if(tLine.size()>=9)tree1 = tLine[8];
					if(tLine.size()>=10)tree2 = tLine[9];
					if(tLine.size()>=11)tree3 = tLine[10];
					if(tLine.size()>=12)treeCount = StringConverter::parseInt(tLine[11]);
					if(tLine.size()>=13)bush1 = tLine[12];
					if(tLine.size()>=14)bush2 = tLine[13];
					if(tLine.size()>=15)bush3 = tLine[14];
					return true;
				}
			}
		}


		return false;
	}*/
---
in external defs a function named processObjects
---
	/*void processObjects(const String &filename)
	{
		long tSize = 0;
		char *tBuffer;
		String tData="";

		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ifstream::in);
		if(inFile.good())
		{
			inFile.seekg(0,std::ios::end);
			tSize = inFile.tellg();
			inFile.seekg(0,std::ios::beg);
			tBuffer = new char[tSize];
			strcpy(tBuffer,"");
			inFile.getline(tBuffer,tSize,'#');
			inFile.close();
			tData = tBuffer;
			delete[] tBuffer;
		}

		vector<String> tMeshname;
		vector<String> tStuff;

		const vector<String> tPart = StringUtil::split(tData,"[#");
		for(int i=0;i<int(tPart.size());i++)
		{
			const vector<String> tLine = StringUtil::split(tPart[i],"\n");
			if(tLine.size()>0)
			{
				if(StringUtil::startsWith(tLine[0],"Object",false))
				{
					String tMesh = tLine[1];
					if(tLine.size()>6)tMesh += ";"+tLine[6];
					tMeshname.push_back(tMesh);
					const Vector3 tPos = StringConverter::parseVector3(tLine[2]);
					String tRestofstuff = StringConverter::toString(tPos);
					tRestofstuff += ";";
					const Vector3 t1 = StringConverter::parseVector3(tLine[3]);
					const Vector3 t2 = StringConverter::parseVector3(tLine[4]);
					tRestofstuff += StringConverter::toString(t1) + ";";
					tRestofstuff += StringConverter::toString(t2);
					tStuff.push_back(tRestofstuff);
				}
			}
		}

		std::ofstream outFile;
		String newFilename = filename.c_str();
		newFilename += ".cfg";
		outFile.open(newFilename.c_str());

		vector<int> tWritten;

		for(int i=0;i<(int)tMeshname.size();i++)
		{
			bool isWritten = false;
			for(int k=0;k<(int)tWritten.size();k++)
			{
				if(tWritten[k]==i)
				{
					isWritten = true;
					break;
				}
			}
			if(isWritten)continue;
			String tBuffer2 = "["+tMeshname[i]+"]\n";
			outFile.write(tBuffer2.c_str(),(int)tBuffer2.length());
			tBuffer2 = tStuff[i]+"\n";
			outFile.write(tBuffer2.c_str(),(int)tBuffer2.length());

			for(int j=i+1;j<(int)tMeshname.size();j++)
			{
				if(tMeshname[i]==tMeshname[j])
				{
					tBuffer2 = tStuff[j]+"\n";
					outFile.write(tBuffer2.c_str(),(int)tBuffer2.length());
					tWritten.push_back(j);
				}
			}
		}

		const String tBuffer2 = "#";
		outFile.write(tBuffer2.c_str(),(int)tBuffer2.length());
		outFile.close();
	}*/
---
from unit manager updateGroundHeight
---
	/*void updateGroundHeight(MagixAnimated *unit, const FrameEvent &evt)
	{
		Real tHeight = 0;
		static Ray updateRay;
		updateRay.setOrigin(unit->getPosition() + Vector3(0,5000,0));
		updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
		mRayQuery->setRay(updateRay);
		RaySceneQueryResult& qryResult = mRayQuery->execute();
		RaySceneQueryResult::iterator i = qryResult.begin();
		if(i != qryResult.end() && i->worldFragment)
		{
			tHeight = i->worldFragment->singleIntersection.y;
		}
		Real tLength = Vector2(unit->getLastPosition().x-unit->getPosition().x,unit->getLastPosition().z-unit->getPosition().z).length();
		if(tLength!=0)
		if((tHeight - unit->getGroundHeight())/tLength>=1)
		{
			unit->setPosition(unit->getLastPosition());
			unit->setForce(Vector3(0,unit->getForce().y,0));
		}
		unit->setGroundHeight(tHeight);
	}*/
---
from char screen manager, old encrypt/decrypt ?
---
	/*void encrypt(String &input)
	{
		for(int i=0;i<(int)input.length();i++)
		{
			input[i] = input[i]+10;
		}
	}
	void decrypt(String &input)
	{
		for(int i=0;i<(int)input.length();i++)
		{
			input[i] = input[i]-10;
		}
	}*/
---
from char screen manager, create and delete char functions
---
/*void createChar(short iID)
{
	using namespace std;

	ofstream outFile;
	outFile.open(FILENAME_CHARACTERS, ios::app);

	//char name
	String tBuffer = getName();
	StringUtil::trim(tBuffer);
	tBuffer += ";";
	//char meshstring
	tBuffer += mUnit->getMeshString() + ";";
	//char matgroupname
	tBuffer += mUnit->getMatGroupName();
	//char scale
	if(mUnit->getObjectNode()->getScale().x != 1)
	{
		tBuffer += ";" + StringConverter::toString(mUnit->getObjectNode()->getScale().x);
	}
	//char colours
	if(mUnit->getMatGroupName() == "CustomMat")
	{
		tBuffer += "|" + mUnit->getColourString();
	}
	encrypt(tBuffer);
	outFile.write(tBuffer.c_str(),int(tBuffer.length()));
	//endline
	tBuffer = "\n";
	outFile.write(tBuffer.c_str(),int(tBuffer.length()));

	outFile.close();
}
void deleteChar(short iID)
{
	using namespace std;

	short tCharID = 0;
	long tSize = 0;
	char *tBuffer;

	//read, read, read
	ifstream inFile;
	inFile.open(FILENAME_CHARACTERS, ifstream::in);

	inFile.seekg(0,ifstream::end);
	tSize = inFile.tellg();
	inFile.seekg(0);

	tBuffer = new char[tSize];
	strcpy(tBuffer,"");

	while(inFile.good() && !inFile.eof())
	{
		char tMiniBuffer[1024]="";
		inFile.getline(tMiniBuffer,1024);
		if(tCharID != iID)
		{
			strcat(tBuffer,tMiniBuffer);
			strcat(tBuffer,"\n");
		}
		tCharID += 1;
	}
	inFile.close();

	//write, write, write
	ofstream outFile;
	outFile.open(FILENAME_CHARACTERS, ofstream::out);
	outFile.write(tBuffer,int(strlen(tBuffer)-1));
	outFile.close();

	delete[] tBuffer;

	setName("");
	mUnit->destroyUnit(mSceneMgr);
	charID = loadNumChars()-1;
	refreshPage();
}*/
---
from machix char screen function loadNumChars
---
/*short loadNumChars()
{
	using namespace std;

	short tNumChars = 0;
	ifstream inFile;
	inFile.open(FILENAME_CHARACTERS, ifstream::in);
	while(inFile.good() && !inFile.eof())
	{
		char tBuffer[1024];
		inFile.getline(tBuffer,1024);
		tNumChars += 1;
	}
	inFile.close();

	return tNumChars;
}*/
/*void loadChar(short iID)
{
	using namespace std;

	char tBuffer[1024]="";
	short tNumChars = -1;
	ifstream inFile;
	inFile.open(FILENAME_CHARACTERS, ifstream::in);

	if(!inFile.good())mAlertBox->showAlert("Click here to create\na new character",0.70,0.85,5);

	while(inFile.good() && !inFile.eof() && tNumChars<iID)
	{
		strcpy(tBuffer,"");
		inFile.getline(tBuffer,1024);
		tNumChars += 1;
	}
	inFile.close();

	String tData = tBuffer;
	decrypt(tData);

	vector<String>::type tString = StringUtil::split(tData,"|\n",2);
	//Loaded successfully
	if(tString.size()>0)
	{
		vector<String>::type tParam = StringUtil::split(tString[0],";");
		if(tParam.size()==7 || tParam.size()==8)
		{
			setName(tParam[0]);
			mUnit->createUnit(1,mSceneMgr,tParam[1],tParam[2],tParam[3],tParam[4],tParam[5],tParam[6]);
			if(tParam.size()==8)
			{
				const float tScale = StringConverter::parseReal(tParam[7]);
				if(tScale>0 && tScale<10)mUnit->getObjectNode()->setScale(tScale,tScale,tScale);
			}
			if(tParam[6]=="CustomMat" && tString.size()==2)mUnit->setColours(tString[1]);
			const Real tDist = (mUnit->getObjectNode()->getScale().x-1)*-11;
			mUnit->setPosition(3215 - (tDist<0?tDist:0)*0.6, 113 + (tDist<0?tDist:0), 2750 + (tDist<0?tDist:0)*0.6);
			mUnit->resetTarget();
			mUnit->setGroundHeight(113 + (tDist<0?tDist:0));
			mUnit->yaw(Degree(-80));
			mUnit->getObjectNode()->yaw(Degree(-80));
			mUnit->setFreeLook(false,false);
			mUnit->update(FrameEvent(),mDef);

			return;
		}
		//bad data, delete it
		if(iID<(loadNumChars()-1))deleteChar(iID);
	}
	//no data
	setName("");
	mUnit->destroyUnit(mSceneMgr);
}*/
---
from magix world in loadworld
---
//if(!mDef->loadWorld(name,tTerrain,worldSize.x,worldSize.y,spawnSquare,tGrassMat,tGrassMap,tGrassColourMap,tTree1,tTree2,tTree3,tTreeCount,tBush1,tBush2,tBush3))return;
---
from magix world in the middle of loadWorld
---
//mDef->processObjects("media/terrains/"+worldName+"/"+worldName+".world");