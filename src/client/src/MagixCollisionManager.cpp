#include "MagixCollisionManager.h"

void MagixCollisionManager::initialize(SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	createSphereMesh("CollSphere", 1);
}

void MagixCollisionManager::createCollSphere(const Vector3& center, const Real& range)
{
	CollSphere tSphere;
	tSphere.center = center;
	tSphere.range = range;
	collSphere.push_back(tSphere);
}
const vector<CollSphere*>::type MagixCollisionManager::getCollSphereHitList(const Vector3& target)
{
	vector<CollSphere*>::type tList;
	list<CollSphere>::type::iterator it = collSphere.begin();
	while (it != collSphere.end())
	{
		CollSphere* tSphere = &*it;
		if (tSphere->collides(target))tList.push_back(tSphere);
		it++;
	}
	return tList;
}