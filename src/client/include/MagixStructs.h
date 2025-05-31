// master "structs" file, to be put in a namespace OR even a GameEngine class later
#ifndef __MagixStructs_h_
#define __MagixStructs_h_

//#include "MagixNetworkDefines.h"
//#include "GameConfig.h"
//#include <OgreStreamSerialiser.h>
//#include <OgreException.h>
#include "Ogre.h"

using namespace Ogre;

struct WeatherEvent
{
	Real start;
	Real end;
	ColourValue skyShader;
	ColourValue skyAdder;
	String type;
	Real rate;
	vector<String>::type effect;
	Real effectFrequency;
	bool isConstant;
	WeatherEvent()
	{
		start = 0;
		end = 7000;
		skyShader = ColourValue::White;
		skyAdder = ColourValue::Black;
		type = "";
		rate = 0;
		effect.clear();
		effectFrequency = 0;
		isConstant = false;
	}
};

struct AttackFX
{
	String bone;
	String trailMat;
	Real trailWidth;
	ColourValue colour;
	ColourValue colourChange;
	String trailHeadMat;
	AttackFX()
	{
		bone = "";
		trailMat = "";
		trailWidth = 2;
		colour = ColourValue(1, 0.9, 0.75);
		colourChange = ColourValue(7, 7, 7, 7);
		trailHeadMat = "";
	}
};
struct Attack
{
	String name;
	String anim;
	vector<AttackFX>::type FX;
	Real range;
	Vector3 hitForce;
	Vector3 offset;
	Real hpMin;
	Real hpMax;
	bool hitAlly;
	bool autoTarget;
	Vector3 moveForce;
	bool singleTarget;
	Real speedMultiplier;
	bool soundRoar;
	unsigned short attackRange;
	Attack()
	{
		name = "";
		anim = "";
		FX.clear();
		range = 0;
		hitForce = Vector3::ZERO;
		offset = Vector3::ZERO;
		hpMin = 0;
		hpMax = 0;
		hitAlly = false;
		autoTarget = true;
		moveForce = Vector3::ZERO;
		singleTarget = false;
		speedMultiplier = 1;
		soundRoar = false;
		attackRange = 0;
	}
};

struct CritterAttack
{
	Real hp;
	Real range;
	Vector3 hitForce;
	bool hitAlly;
	CritterAttack(const Real& h, const Real& r, const Vector3& hF, bool hA = false)
	{
		hp = h;
		range = r;
		hitForce = hF;
		hitAlly = hA;
	}
};

struct Critter
{
	String type;
	String mesh;
	Real hp;
	bool flying;
	bool friendly;
	bool invulnerable;
	bool isDrawPoint;
	bool isUncustomizable;
	vector<std::pair<String, Real>>::type dropList;
	std::pair<String, unsigned char> skillDrop;
	Real maxSpeed;
	unsigned char decisionMin;
	unsigned char decisionDeviation;
	Real scale;
	vector<CritterAttack>::type attackList;
	String sound;
	String material;
	Critter()
	{
		type = "";
		mesh = "";
		hp = 100;
		flying = false;
		friendly = false;
		invulnerable = false;
		isDrawPoint = false;
		isUncustomizable = false;
		dropList.clear();
		skillDrop = std::pair<String, unsigned char>("", 0);
		maxSpeed = 100;
		decisionMin = 1;
		decisionDeviation = 9;
		scale = 1;
		attackList.clear();
		sound = "";
		material = "";
	}
};

struct WorldCritter
{
	String type;
	Real spawnRate;
	bool hasRoamArea;
	unsigned short roamAreaID;
	WorldCritter(const String& t = "", const Real& sR = 0, bool hRA = false, const unsigned short& rAID = 0)
	{
		type = t;
		spawnRate = sR;
		hasRoamArea = hRA;
		roamAreaID = rAID;
	}
};
#endif;