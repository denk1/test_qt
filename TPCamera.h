#ifndef TPCamera_h
#define TPCamera_h

#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreSingleton.h>
#include "ITS.h"

namespace RAT
{
class ITS;
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
    void changeSignDirection(int sign);
    bool setTargetFollowing();

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
    ITS& mIts;
    int mSignDirection;

};

}

#endif
