#ifndef TPCamera_h
#define TPCamera_h

#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreSingleton.h>
#include "MyTestApp.h"

namespace RAT
{

class TPCamera : public Ogre::Singleton<TPCamera>, public Ogre::FrameListener
{
public:

	TPCamera();
	~TPCamera();

	//void setActiveCamera(Ogre::Camera* camera);
	Ogre::Camera* getCamera();

	// static version to reduce code length
	static Ogre::Camera* getCameraS();

	void rotate(Ogre::Real yaw, Ogre::Real pitch);

	void setTarget(Ogre::SceneNode* target, const Ogre::Real dist = 0.0f);
	void setTargetVehicle(Ogre::SceneNode* target, const Ogre::Real dist = 0.0f);

	void startGoingRight();
	void startGoingLeft();
	void startGoingForward();
	void startGoingBack();

	void stopGoingRight();
	void stopGoingLeft();
	void stopGoingForward();
	void stopGoingBack();

	void update();

protected:

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	Ogre::Camera* mCamera;
	Ogre::Real mVelocity;
	Ogre::Real mSencivity;

	bool mGoingRight;
	bool mGoingLeft;
	bool mGoingForward;
	bool mGoingBack;

	Ogre::SceneNode* mTarget;
	Ogre::Real mTargetDistance;
	Ogre::Real mYaw;
	Ogre::Real mPitch;
        bool mFollowVehicle;
        MyTestApp& myTestApp;

};

}

#endif
