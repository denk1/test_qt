#ifndef Config_H
#define Config_H

#include <Ogre.h>
#include <OgreSingleton.h>


namespace RAT
{

class Config : public Ogre::Singleton<Config>
{
public:

	Config();
	~Config();

	void setCameraDistance(const float value);
	float getCameraDistance();

protected:
	float mCameraDistance;

};

}

#endif