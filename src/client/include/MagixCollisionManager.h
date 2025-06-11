#ifndef __MagixCollisionManager_h_
#define __MagixCollisionManager_h_

#include "Ogre.h"
#include "MagixStructs.h"

using namespace Ogre;




class MagixCollisionManager
{
protected:
	SceneManager *mSceneMgr;
	
	list<Wall>::type wall;
	list<Portal>::type portal;
	list<Gate>::type gate;
	list<WaterBox>::type waterBox;
	list<CollBox>::type collBox;
	list<CollSphere>::type collSphere;
public:
	list<Collision>::type coll;
	MagixCollisionManager()
	{
		mSceneMgr = 0;
		coll.clear();
		wall.clear();
		portal.clear();
		gate.clear();
		waterBox.clear();
		collBox.clear();
		collSphere.clear();
	}
	~MagixCollisionManager()
	{
		reset();
		destroyAllWaterBoxes();
		destroyAllPortals();
		destroyAllGates();
	}

	void initialize(SceneManager* sceneMgr);
	//{
	//	mSceneMgr = sceneMgr;
	//	createSphereMesh("CollSphere",1);
	//}
	void reset()
	{
		coll.clear();
		wall.clear();
		collBox.clear();
		collSphere.clear();
	}
	void createSphereMesh(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16)
	{
		MeshPtr pSphere = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		SubMesh *pSphereVertex = pSphere->createSubMesh();

		pSphere->sharedVertexData = new VertexData();
		VertexData* vertexData = pSphere->sharedVertexData;

		// define the vertex format
		VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
		size_t currOffset = 0;
		// positions
		vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
		currOffset += VertexElement::getTypeSize(VET_FLOAT3);
		// normals
		vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
		currOffset += VertexElement::getTypeSize(VET_FLOAT3);
		// two dimensional texture coordinates
		vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
		currOffset += VertexElement::getTypeSize(VET_FLOAT2);

		// allocate the vertex buffer
		vertexData->vertexCount = (nRings + 1) * (nSegments+1);
		HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		VertexBufferBinding* binding = vertexData->vertexBufferBinding;
		binding->setBinding(0, vBuf);
		float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

		// allocate index buffer
		pSphereVertex->indexData->indexCount = 6 * nRings * (nSegments + 1);
		pSphereVertex->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
		unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));

		float fDeltaRingAngle = (Math::PI / nRings);
		float fDeltaSegAngle = (2 * Math::PI / nSegments);
		unsigned short wVerticeIndex = 0 ;

		// Generate the group of rings for the sphere
		for( int ring = 0; ring <= nRings; ring++ ) {
			float r0 = r * sinf (ring * fDeltaRingAngle);
			float y0 = r * cosf (ring * fDeltaRingAngle);

			// Generate the group of segments for the current ring
			for(int seg = 0; seg <= nSegments; seg++) {
				float x0 = r0 * sinf(seg * fDeltaSegAngle);
				float z0 = r0 * cosf(seg * fDeltaSegAngle);

				// Add one vertex to the strip which makes up the sphere
				*pVertex++ = x0;
				*pVertex++ = y0;
				*pVertex++ = z0;

				Vector3 vNormal = Vector3(x0, y0, z0).normalisedCopy();
				*pVertex++ = vNormal.x;
				*pVertex++ = vNormal.y;
				*pVertex++ = vNormal.z;

				*pVertex++ = (float) seg / (float) nSegments;
				*pVertex++ = (float) ring / (float) nRings;

				if(ring != nRings) {
								   // each vertex (except the last) has six indices pointing to it
					*pIndices++ = wVerticeIndex + nSegments + 1;
					*pIndices++ = wVerticeIndex;               
					*pIndices++ = wVerticeIndex + nSegments;
					*pIndices++ = wVerticeIndex + nSegments + 1;
					*pIndices++ = wVerticeIndex + 1;
					*pIndices++ = wVerticeIndex;
					wVerticeIndex ++;
				}
			}; // end for seg
		} // end for ring

		// Unlock
		vBuf->unlock();
		iBuf->unlock();
		// Generate face list
		pSphereVertex->useSharedVertices = true;

		// the original code was missing this line:
		pSphere->_setBounds( AxisAlignedBox( Vector3(-0.1, -0.1, -0.1), Vector3(0.1, 0.1, 0.1) ), false );
		pSphere->_setBoundingSphereRadius(r);
		// this line makes clear the mesh is loaded (avoids memory leaks)
		pSphere->load();
	 }
	void createCollision(SceneNode *node, const Real &range, const Vector3 &force, const Vector3 &offset=Vector3::ZERO, const short &hp=0, const unsigned char &alliance=0, bool hitAlly=false)
	{
		Collision tColl;
		tColl.mNode = node;
		tColl.offset = offset;
		tColl.range = range;
		tColl.force = force;
		tColl.hp = hp;
		tColl.alliance = alliance;
		tColl.hitAlly = hitAlly;
		coll.push_back(tColl);
	}
	const list<Collision>::type::iterator destroyCollision(const list<Collision>::type::iterator &it)
	{
		return coll.erase(it);
	}
	void destroyCollisionByOwnerNode(SceneNode *node)
	{
		list<Collision>::type::iterator it = coll.begin();
		while(it!=coll.end())
		{
			Collision *tColl = &*it;
			if(tColl->mNode==node)it = destroyCollision(it);
			else it++;
		}
	}
	const vector<Collision*>::type getCollisionHitList(const unsigned short &unitID, const unsigned short &alliance, const AxisAlignedBox &target)
	{
		vector<Collision*>::type tList;
		list<Collision>::type::iterator it = coll.begin();
		while(it!=coll.end())
		{
			Collision *tColl = &*it;
			if(tColl->hitAlly && tColl->alliance==alliance || !tColl->hitAlly && tColl->alliance!=alliance)
			if(!tColl->hasHitID(unitID))
			if(tColl->collides(target))
			{
				tColl->hitID.push_back(unitID);
				tList.push_back(tColl);
			}

			it++;
		}
		return tList;
	}
	const vector<Collision*>::type getCollisionHitListForCritter(const unsigned short &iID, const unsigned short &alliance, const AxisAlignedBox &target)
	{
		vector<Collision*>::type tList;
		list<Collision>::type::iterator it = coll.begin();
		while(it!=coll.end())
		{
			Collision *tColl = &*it;
			if(tColl->hitAlly && tColl->alliance==alliance || !tColl->hitAlly && tColl->alliance!=alliance)
			if(!tColl->hasCritterHitID(iID))
			if(tColl->collides(target))
			{
				tColl->critterHitID.push_back(iID);
				tList.push_back(tColl);
			}

			it++;
		}
		return tList;
	}
	void createBoxWall(const Vector3 &center, const Vector3 &range, bool isInside)
	{
		Wall tWall;
		tWall.isActive = true;
		tWall.isSphere = false;
		tWall.isInside = isInside;
		tWall.center = center;
		tWall.range = range;
		wall.push_back(tWall);
	}
	void createSphereWall(const Vector3 &center, const Real &range, bool isInside)
	{
		Wall tWall;
		tWall.isActive = true;
		tWall.isSphere = true;
		tWall.isInside = isInside;
		tWall.center = center;
		tWall.range = Vector3(range,0,0);
		wall.push_back(tWall);
	}
	const list<Wall>::type::iterator destroyWall(const list<Wall>::type::iterator &it)
	{
		return wall.erase(it);
	}
	void destroyWall(const unsigned short &iID)
	{
		unsigned short tID = 0;
		list<Wall>::type::iterator it = wall.begin();
		while(it!=wall.end())
		{
			if(tID==iID){wall.erase(it);return;}
			it++;
			tID++;
		}
	}
	const vector<Wall*>::type getWallHitList(const Vector3 &target)
	{
		vector<Wall*>::type tList;
		list<Wall>::type::iterator it = wall.begin();
		while(it!=wall.end())
		{
			Wall *tWall = &*it;
			if(tWall->collides(target))tList.push_back(tWall);
			it++;
		}
		return tList;
	}
	void createPortal(const Vector3 &center, const Real &range, String destName)
	{
		if(!mSceneMgr)return;

		Portal tPortal;
		tPortal.mEnt = mSceneMgr->createEntity("PortalSphere"+StringConverter::toString(portal.size()+1),"CollSphere");
		tPortal.mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		tPortal.mNode->attachObject(tPortal.mEnt);
		//tPortal.mNode->setInheritScale(false);
		tPortal.mNode->setScale(range,range,range);
		//Ogre::Sphere t = tPortal.mEnt->getWorldBoundingSphere(true);
		tPortal.mNode->setPosition(center);
		//Ogre::Node* n = tPortal.mEnt->getParentNode();
		
		//n->setScale(range,range,range);
		/*const Vector3& scl = n->_getDerivedScale();
		Real factor = std::max(std::max(scl.x, scl.y), scl.z);
		Real bRadius = tPortal.mEnt->getBoundingRadius();
		Ogre::Sphere t = tPortal.mEnt->getWorldBoundingSphere();
		Ogre::LogManager::getSingleton().logMessage("Portal to: "+destName+ " Radius: "+Ogre::StringConverter::toString(tPortal.mEnt->getWorldBoundingSphere().getRadius()));*/
		//Sphere mSph = tPortal.mEnt->getWorldBoundingSphere();
		//mSph.setRadius((Real)0.00025);
		tPortal.mNode->_updateBounds();
		//tPortal.mNode->showBoundingBox(true);
		tPortal.dest = destName;
		for(int i=0;i<(int)destName.length();i++)if(destName[i]==' ')destName[i] = '_';
		tPortal.mEnt->setMaterialName("Portal/"+destName);
		portal.push_back(tPortal);
	}
	const list<Portal>::type::iterator destroyPortal(list<Portal>::type::iterator it)
	{
		if(!mSceneMgr)return ++it;
		Portal *tPortal = &*it;
		if(tPortal->mEnt && mSceneMgr->hasEntity(tPortal->mEnt->getName()))
		{
			tPortal->mNode->detachObject(tPortal->mEnt);
			mSceneMgr->getRootSceneNode()->removeChild(tPortal->mNode);
			mSceneMgr->destroyEntity(tPortal->mEnt);
			mSceneMgr->destroySceneNode(tPortal->mNode->getName());
		}
		return portal.erase(it);
	}
	void destroyAllPortals()
	{
		list<Portal>::type::iterator it = portal.begin();
		while(it!=portal.end())
		{
			it = destroyPortal(it);
		}
	}
	Portal* getPortalHit(const AxisAlignedBox &target)
	{
		list<Portal>::type::iterator it = portal.begin();
		while(it!=portal.end())
		{
			Portal *tPortal = &*it;
			if(tPortal->collides(target))return tPortal;
			it++;
		}
		return 0;
	}
	void getPortalMap(vector<std::pair<Vector2,String>>::type &map)
	{
		map.clear();
		list<Portal>::type::iterator it = portal.begin();
		while(it!=portal.end())
		{
			Portal *tPortal = &*it;
			if(tPortal->mNode)
			{
				const std::pair<Vector2,String> tData = std::pair<Vector2,String>(Vector2(tPortal->mNode->getPosition().x,tPortal->mNode->getPosition().z),tPortal->dest);
				map.push_back(tData);
			}
			it++;
		}
	}
	void createWaterBox(const Vector3 &center, const Real &scaleX, const Real &scaleZ, bool isSolid=false)
	{
		WaterBox tBox;
		tBox.isActive = true;
		tBox.center = center;
		tBox.scale = Vector2(scaleX,scaleZ);
		tBox.isSolid = isSolid;
		waterBox.push_back(tBox);
	}
	const list<WaterBox>::type::iterator destroyWaterBox(const list<WaterBox>::type::iterator &it)
	{
		return waterBox.erase(it);
	}
	void destroyAllWaterBoxes()
	{
		list<WaterBox>::type::iterator it = waterBox.begin();
		while(it!=waterBox.end())
		{
			it = destroyWaterBox(it);
		}
	}
	WaterBox* getWaterBoxHit(const Vector3 &target)
	{
		list<WaterBox>::type::iterator it = waterBox.begin();
		while(it!=waterBox.end())
		{
			WaterBox *tBox = &*it;
			if(tBox->collides(target))return tBox;
			it++;
		}
		return 0;
	}
	void createGate(const Vector3 &center, String destName, const Vector3 &destVect, const String &matName, bool hasVectY)
	{
		if(!mSceneMgr)return;

		Gate tGate;
		tGate.mEnt = mSceneMgr->createEntity("GateSphere"+StringConverter::toString(gate.size()+1),"CollSphere");
		tGate.mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		tGate.mNode->attachObject(tGate.mEnt);
		//tGate.mNode->setInheritScale(false);
		tGate.mNode->setScale(40,40,40);
		tGate.mNode->setPosition(center);
		/*Ogre::Node* n = tGate.mEnt->getParentNode();
		n->setScale(40,40,40);
		const Vector3& scl = n->_getDerivedScale();
		Real factor = std::max(std::max(scl.x, scl.y), scl.z);
		Real bRadius = tGate.mEnt->getBoundingRadius();
		Ogre::Sphere t = tGate.mEnt->getWorldBoundingSphere();*/
		//Sphere mSph = tGate.mEnt->getWorldBoundingSphere();
		//mSph.setRadius((Real)0.00025);
		tGate.mNode->_updateBounds();
		tGate.dest = destName;
		tGate.destVect = destVect;
		tGate.hasVectY = hasVectY;
		tGate.mEnt->setMaterialName(matName);
		tGate.mEnt->setCastShadows(false);
		gate.push_back(tGate);
	}
	const list<Gate>::type::iterator destroyGate(list<Gate>::type::iterator it)
	{
		if(!mSceneMgr)return ++it;
		Gate *tGate = &*it;
		if(tGate->mEnt && mSceneMgr->hasEntity(tGate->mEnt->getName()))
		{
			tGate->mNode->detachObject(tGate->mEnt);
			mSceneMgr->getRootSceneNode()->removeChild(tGate->mNode);
			mSceneMgr->destroyEntity(tGate->mEnt);
			mSceneMgr->destroySceneNode(tGate->mNode->getName());
		}
		return gate.erase(it);
	}
	void destroyAllGates()
	{
		list<Gate>::type::iterator it = gate.begin();
		while(it!=gate.end())
		{
			it = destroyGate(it);
		}
	}
	Gate* getGateHit(const AxisAlignedBox &target)
	{
		list<Gate>::type::iterator it = gate.begin();
		while(it!=gate.end())
		{
			Gate *tGate = &*it;
			if(tGate->collides(target))return tGate;
			it++;
		}
		return 0;
	}
	void getGateMap(vector<std::pair<Vector2,String>>::type &map)
	{
		map.clear();
		list<Gate>::type::iterator it = gate.begin();
		while(it!=gate.end())
		{
			Gate *tGate = &*it;
			if(tGate->mNode)
			{
				const std::pair<Vector2,String> tData = std::pair<Vector2,String>(Vector2(tGate->mNode->getPosition().x,tGate->mNode->getPosition().z),tGate->dest);
				map.push_back(tData);
			}
			it++;
		}
	}
	void createCollBox(const Vector3 &center, const Vector3 &range)
	{
		CollBox tBox;
		tBox.center = center;
		tBox.range = range;
		collBox.push_back(tBox);
	}
	const vector<CollBox*>::type getCollBoxHitList(const Vector3 &target, const Real &headHeight)
	{
		vector<CollBox*>::type tList;
		list<CollBox>::type::iterator it = collBox.begin();
		while(it!=collBox.end())
		{
			CollBox *tBox = &*it;
			if(tBox->collides(target,headHeight))tList.push_back(tBox);
			it++;
		}
		return tList;
	}
	void createCollSphere(const Vector3& center, const Real& range);

	const vector<CollSphere*>::type getCollSphereHitList(const Vector3& target);
};

#endif