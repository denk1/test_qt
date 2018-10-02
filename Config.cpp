#include "Config.h"

template<> RAT::Config* Ogre::Singleton<RAT::Config>::msSingleton = 0;

namespace RAT
{

Config::Config()
	: mCameraDistance(10.f)
{
	
}

Config::~Config()
{
	
}

void Config::setCameraDistance(const float value)
{
	mCameraDistance = value;
}

float Config::getCameraDistance()
{
	return mCameraDistance;
}

}
