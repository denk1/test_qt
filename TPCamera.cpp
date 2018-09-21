#include "TPCamera.h"
#include "MyTestApp.h"

template<> RAT::TPCamera* Ogre::Singleton<RAT::TPCamera>::msSingleton = 0;

namespace RAT
{

TPCamera::TPCamera()
	:mVelocity(60), mSencivity(0.001), mYaw(0), mPitch(0),
    mGoingBack(false), mGoingForward(false), mGoingLeft(false), mGoingRight(false), mTarget(0), mFollowVehicle(false),  myTestApp(MyTestApp::getSingleton())
{
    mCamera = myTestApp.getSceneManager()->createCamera("PlayerCam");
	
	mCamera->setPosition(Ogre::Vector3(0, 200, 200));
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(1);
	mCamera->setCastShadows(false);
	//mCamera->setAutoAspectRatio(true);
	mCamera->setAspectRatio(
        Ogre::Real(myTestApp.getWindow()->getWidth()) / Ogre::Real(myTestApp.getWindow()->getHeight()));
	
	mCamera->setOrientation(Ogre::Quaternion(Ogre::Radian(mPitch),Ogre::Vector3::UNIT_Y)*Ogre::Quaternion(Ogre::Radian(mYaw),Ogre::Vector3::UNIT_X));
}

TPCamera::~TPCamera()
{
}

void TPCamera::setTarget(Ogre::SceneNode* target, const Ogre::Real dist)
{
	mTarget = target;
	mFollowVehicle = false;
	mTargetDistance = dist;

	mYaw = 0;
	mPitch = 0;
}

void TPCamera::setTargetVehicle(Ogre::SceneNode* target, const Ogre::Real dist)
{
	mTarget = target;
	mFollowVehicle = true;
	mTargetDistance = dist;

	mYaw = 0;
	mPitch = 0;
}

void TPCamera::update()
{
	if (mTarget)
	{
		if (mFollowVehicle)
		{

			Ogre::Vector3 dir = mTarget->_getDerivedOrientation() * Ogre::Quaternion(Ogre::Radian(mYaw), Ogre::Vector3::UNIT_Y) *Ogre::Quaternion(Ogre::Radian(mPitch), Ogre::Vector3::UNIT_X) * Ogre::Vector3(0,0,1);
			dir.normalise();
			mCamera->setPosition(mTarget->_getDerivedPosition() + Ogre::Vector3(0, 3.0f, 0) + dir*mTargetDistance);
			mCamera->lookAt(mTarget->_getDerivedPosition() + Ogre::Vector3(0, 3.0f, 0));
			
		}
		else
		{
			// TODO: trac for non vehicle
		}
	}
}

bool TPCamera::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mTarget)
	{
		update();
	}
	else
	{
		Ogre::Vector3 delta = Ogre::Vector3::ZERO;

		if (mGoingRight) delta += mCamera->getRight();
		if (mGoingLeft) delta -= mCamera->getRight();
		if (mGoingForward) delta += mCamera->getDirection();
		if (mGoingBack) delta -= mCamera->getDirection();

		delta.normalise();

        if (MyTestApp::getSingleton().getKeyboard()->isModifierDown(OIS::Keyboard::Modifier::Shift))
			delta *= 4;

		mCamera->move(delta * mVelocity * evt.timeSinceLastFrame);
	}
		
	return true;
}


Ogre::Camera* TPCamera::getCamera()
{
	return mCamera;
}

Ogre::Camera* TPCamera::getCameraS()
{
	return TPCamera::getSingleton().mCamera;
}



void TPCamera::rotate(Ogre::Real yaw, Ogre::Real pitch)
{
	mCamera->yaw(Ogre::Radian(-yaw*mSencivity));
	mCamera->pitch(Ogre::Radian(-pitch*mSencivity));
	
	if (mTarget)
	{
		mYaw -= yaw*mSencivity;
		mPitch -= pitch*mSencivity;
	}
	
}

void TPCamera::startGoingRight()
{
	mGoingRight = true;
}

void TPCamera::startGoingLeft()
{
	mGoingLeft = true;
}

void TPCamera::startGoingForward()
{
	mGoingForward = true;
}

void TPCamera::startGoingBack()
{
	mGoingBack = true;
}

void TPCamera::stopGoingRight()
{
	mGoingRight = false;
}

void TPCamera::stopGoingLeft()
{
	mGoingLeft = false;
}

void TPCamera::stopGoingForward()
{
	mGoingForward = false;
}

void TPCamera::stopGoingBack()
{
	mGoingBack = false;
}

}
