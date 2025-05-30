trashpad
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
