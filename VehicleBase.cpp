#include "VehicleBase.h"
#include "ITS.h"
#include "Config.h"

namespace RAT
{

VehicleBase::VehicleBase(const VehiclePrototype& prototype)
    :VehiclePrototype(prototype), mSteeringLeft (false), mSteeringRight(false), mGas(false), mBreaks(false), mHandBreaks(false), mSpeed(0), mSpeedPos(0), mSkipedTime(0)
    , mSpeedToKmh(0.5f * 3.6f) // 0.5 - ?? ???????? ? ?????, 3.6 - ?? ?/c ? ??/?
    , mSpeedToMph(0.5f * 2.236936f) // 0.5 - ?? ???????? ? ?????, 2.236936 - ?? ?/c ? ?/?
    , mAimAngle(.0)
    , mCoeffForce(.0)
{
    mActionMap[VSNone] = &VehicleBase::processNone;
    mActionMap[VSLeftPressed] = &VehicleBase::processLeftPressed;
    mActionMap[VSRightPressed] = &VehicleBase::processRightPressed;
    mActionMap[VSForwardPressed] = &VehicleBase::processForwardPressed;
    mActionMap[VSBackwardPressed] = &VehicleBase::processBackwardPressed;
    mActionMap[VSHandBreakPressed] = &VehicleBase::processHandBreakPressed;

    mActionMap[VSLeftReleased] = &VehicleBase::processLeftReleased;
    mActionMap[VSRightReleased] = &VehicleBase::processRightReleased;
    mActionMap[VSForwardReleased] = &VehicleBase::processForwardReleased;
    mActionMap[VSBackwardReleased] = &VehicleBase::processBackwardReleased;
    mActionMap[VSHandBreakReleased] = &VehicleBase::processHandBreakReleased;
}

VehicleBase::~VehicleBase()
{
    if (mVehicleSN)
    {
        TPCamera::getSingleton().setTargetVehicle(0);

        delete mCarChassis;
        delete mVehicleRayCaster;
        delete mVehicle;
        delete mTuning;

        // TODO: delete physics shapes

        Ogre::SceneNode* node = mVehicleSN;
        ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), node);
        node->removeAndDestroyAllChildren();
        ITS::getSceneManagerS()->destroySceneNode(node);

        for(int i = 0; i < 4; ++i)
        {
            node = mVehicleWSN[i];
            ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), node);
            node->removeAndDestroyAllChildren();
            ITS::getSceneManagerS()->destroySceneNode(node);
        }

    }
}

void VehicleBase::create(Ogre::SceneNode* parentSN)
{
    float limit = mProperties.mSteeringClamp;
    mSteering = 0;
    mVehicleSN = parentSN->createChildSceneNode();
    Ogre::Entity* entity = ITS::getSceneManagerS()->createEntity(mMesh);

    mBodySN = mVehicleSN->createChildSceneNode();
    mBodySN->attachObject(entity);
    mBodySN->yaw(Ogre::Degree(180));
    mCarChassis = new OgreBulletDynamics::WheeledRigidBody(mName + "_chassis", Physics::getSingleton().getWorld());
    if(mShape.get() != 0)
    {
        //mCarChassis->setShape(mVehicleSN, mShape.get(), mProperties.mRestitution, mProperties.mFriction, mProperties.mMass, Ogre::Vector3(350,75,350));
        mCarChassis->setShape(mVehicleSN, mShape.get(), mProperties.mRestitution, mProperties.mFriction, mProperties.mMass, Ogre::Vector3(461.50, 10, 448.0));
    }
    else
    {
        const Ogre::Vector3 chassisShift(0.0f, 1.2, -0.3f);
        OgreBulletCollisions::BoxCollisionShape* chassisShape = new OgreBulletCollisions::BoxCollisionShape(entity->getBoundingBox().getHalfSize()*0.96f);
        OgreBulletCollisions::CompoundCollisionShape* compound = new OgreBulletCollisions::CompoundCollisionShape();
        compound->addChildShape(chassisShape, chassisShift);
        mCarChassis->setShape (mVehicleSN, compound, mProperties.mRestitution, mProperties.mFriction, mProperties.mMass, Ogre::Vector3(350,75,350));
    }

    mCarChassis->setDamping(mProperties.mLinearDamping, mProperties.mAngularDamping);

    mCarChassis->disableDeactivation ();

    mTuning = new OgreBulletDynamics::VehicleTuning(
            mProperties.mSuspensionStiffness,
            mProperties.mSuspensionCompression,
            mProperties.mSuspensionDamping,
            mProperties.mMaxSuspensionTravelCm,
            mProperties.mFrictionSlip,
            mProperties.mMaxSuspensionForce);

    mVehicleRayCaster = new OgreBulletDynamics::VehicleRayCaster(Physics::getSingleton().getWorld());
    mVehicle = new OgreBulletDynamics::RaycastVehicle(mCarChassis, mTuning, mVehicleRayCaster);

    mVehicle->setCoordinateSystem(0, 1, 2);

    Ogre::Vector3 wheelDirectionCS0(0,-1,0);
    Ogre::Vector3 wheelAxleCS(-1,0, 0);

    for(int i = 0; i < 4; ++i)
    {
        // ??????? ????????? ? ???????? ?? ?? ?????
        mVehicleWSN[i] = parentSN->createChildSceneNode();
        Ogre::SceneNode* node = mVehicleWSN[i]->createChildSceneNode();
        node->attachObject(ITS::getSceneManagerS()->createEntity(mWheels[i].mMesh));
        if (!(i % 2))
            node->yaw(Ogre::Degree(180));


        // ????????????? ?????? ??????
        bool isFrontWheel = i < 2;
        float signX = i % 2 ? 1 : -1;
        float signZ = i < 2 ? 1 : -1;

        Ogre::Vector3 connectionPointCS0(
            signX*(mProperties.mWeelOffsetW-(0.3*mProperties.mWheelWidth)),
                        mProperties.mWeelOffsetH,
                        signZ*(mProperties.mWeelOffsetL-mProperties.mWheelRadius));

        mVehicle->addWheel(
            mVehicleWSN[i],
            connectionPointCS0,
            wheelDirectionCS0,
            wheelAxleCS,
            mProperties.mSuspensionRestLength,
            mProperties.mWheelRadius,
            isFrontWheel, mProperties.mWheelFriction, mProperties.mRollInfluence);

    }

    TPCamera::getSingleton().setTargetVehicle(mVehicleSN, Config::getSingleton().getCameraDistance());

    // ??????? SceneNode ??? ?????????

    for (std::vector<WCPoint>::iterator it = mConnectionPoints.begin(); it != mConnectionPoints.end(); ++it)
    {
        Ogre::SceneNode* node = mBodySN->createChildSceneNode(it->mPosition, it->mRotation);
        mCPNodes.push_back(node);
    }
}

Ogre::SceneNode* VehicleBase::getVehicleSN()
{
    return mVehicleSN;
}


void VehicleBase::processControl(const VehicleSteer steer)
{
    (this->*mActionMap[steer])();
}

void VehicleBase::processRightPressed()
{
    mSteeringRight = true;
}
void VehicleBase::processLeftPressed()
{
    mSteeringLeft = true;
}
void VehicleBase::processForwardPressed()
{
    mGas = true;
}
void VehicleBase::processBackwardPressed()
{
    mBreaks = true;
}

void VehicleBase::processHandBreakPressed()
{
    mHandBreaks = true;
}

void VehicleBase::processLeftReleased()
{
    mSteeringLeft = false;
}
void VehicleBase::processRightReleased()
{
    mSteeringRight = false;
}
void VehicleBase::processForwardReleased()
{
    mGas = false;
}
void VehicleBase::processBackwardReleased()
{
    mBreaks = false;
}
void VehicleBase::processHandBreakReleased()
{
    mHandBreaks = false;
}

Ogre::Real VehicleBase::getSpeed(const SpeedType speedType)
{
    if (speedType == Units)
        return mSpeed;

    if (speedType == Kmh)
        return mSpeed * mSpeedToKmh;

    if (speedType == Mph)
        return mSpeed * mSpeedToMph;
}

Ogre::Real VehicleBase::getAvgSpeed(const SpeedType speedType)
{
    if (speedType == Units)
        return mAvgSpeed;

    if (speedType == Kmh)
        return mAvgSpeed * mSpeedToKmh;

    if (speedType == Mph)
        return  mAvgSpeed * mSpeedToMph;
}

const Ogre::Vector3& VehicleBase::getDirection()
{
    return mDirection;
}

void VehicleBase::updateVehicle(const Ogre::Real dt)
{
    // ????????? ???????????
    mDirection = mVehicleSN->getOrientation() * Ogre::Vector3(0,0,1);

    if ((dt + mSkipedTime) < 0.0001)
    {
        // ?????????? ??????? ??????? ????
        mSkipedTime += dt;
        return;
    }

    // ????????? ???????????????? ??????????? ? ????????
    Ogre::Vector3 delta = mVehicleSN->getPosition() - mPreviosPosition;

    if (delta.dotProduct(mDirection) > 0)
        mSpeed = delta.length() / (dt + mSkipedTime);
    else
        mSpeed = -delta.length() / (dt + mSkipedTime);

    mSkipedTime = 0;
    mSpeedPos++;
    if (mSpeedPos >= avgSpeedSoft)
        mSpeedPos = 0;

    mLastSpeed[mSpeedPos] = mSpeed;
    mAvgSpeed = 0;
    for(int i=0;i < avgSpeedSoft; i++)
        mAvgSpeed += mLastSpeed[i];
    mAvgSpeed = mAvgSpeed / (Ogre::Real)avgSpeedSoft;

    mPreviosPosition = mVehicleSN->getPosition();

}

void VehicleBase::processGas(const bool forward)
{
    //float forceCoef = 0.1f+0.9f*mSpeed*mSpeed/(100.f*100.f);
    //float forceCoef = 1.f;

    if (forward)
        for(int i = 0; i < 4; ++i)
        {
            //float force = mWheels[i].mEngineForce * forceCoef;
            mVehicle->applyEngineForce(mWheels[i].mEngineForce * mCoeffForce, i);
        }
    else
        for(int i = 0; i < 4; ++i)
            mVehicle->applyEngineForce(-mWheels[i].mEngineBackForce * mCoeffForce, i);

    for(int i = 0; i < 4; ++i)
        mVehicle->getBulletVehicle()->setBrake(0, i);
}

void VehicleBase::processBreaks()
{
    for(int i = 0; i < 4; ++i)
        mVehicle->applyEngineForce(0,i);

    for(int i = 0; i < 4; ++i)
        mVehicle->getBulletVehicle()->setBrake(mWheels[i].mBreakForce, i);
}

void VehicleBase::processIdle()
{
    for(int i = 0; i < 4; ++i)
    {
        mVehicle->applyEngineForce(0,i);
        mVehicle->getBulletVehicle()->setBrake(0, i);
    }
}

void VehicleBase::processHandBreaks()
{
    mVehicle->getBulletVehicle()->setBrake(mWheels[VehiclePrototype::VRLWheelIndex].mHandBreakForce, VehiclePrototype::VRLWheelIndex);
    mVehicle->getBulletVehicle()->setBrake(mWheels[VehiclePrototype::VRRWheelIndex].mHandBreakForce, VehiclePrototype::VRRWheelIndex);
}

void VehicleBase::updateVehicleSteering(const Ogre::Real dt)
{
    float speedAffectionClamp = 1.f / (1.f + mSpeed*mSpeed / (mProperties.mSpeedClampAffection * mProperties.mSpeedClampAffection));
    float speedAffectionIncrement = 1.f / (1.f + mSpeed*mSpeed / (mProperties.mSpeedIncrementAffection * mProperties.mSpeedIncrementAffection));

    if ((mSteeringLeft == mSteeringRight) && ( mSteeringRight == false))
            // ???? ??????? ? ???????????? ? ???????? ?????????
        if (Ogre::Math::Abs(mSteering) < 0.01)
            mSteering = 0;
        else
        {
            if (mSteering > 0)
                mSteering -= mProperties.mSteeringIncrement * 2.f * dt * speedAffectionIncrement;
            else
                mSteering += mProperties.mSteeringIncrement * 2.f * dt * speedAffectionIncrement;

        }
    else
    {
            // ???? ?????????
        if (mSteeringLeft)
        {
            // ?????
            if (mSteering < 0)
                mSteering +=  mProperties.mSteeringIncrement * 4.f * dt * speedAffectionIncrement; // ???? ?????? ????????? ? ?????? ???????, ?? ??????? ??????????
            else
                mSteering +=  mProperties.mSteeringIncrement * dt  * speedAffectionIncrement;

            float limit = mProperties.mSteeringClamp * speedAffectionClamp;

            if(mAimAngle < limit )
                limit = mAimAngle;

            if (mSteering > limit)
                    mSteering = limit;
        }

        if (mSteeringRight)
        {
            // ??????
            if (mSteering > 0)
                mSteering -= mProperties.mSteeringIncrement * 4.f * dt * speedAffectionIncrement; // ???? ?????? ????????? ? ?????? ???????, ?? ??????? ??????????
            else
                mSteering -= mProperties.mSteeringIncrement * dt * speedAffectionIncrement;

            float limit = -mProperties.mSteeringClamp * speedAffectionClamp;

            if(mAimAngle > limit )
                limit = mAimAngle;

            if(mSteering < limit)
                    mSteering = limit;
        }
    }

    mVehicle->setSteeringValue (mSteering, VehiclePrototype::VFLWheelIndex);
    mVehicle->setSteeringValue (mSteering, VehiclePrototype::VFRWheelIndex);

    if (mGas != mBreaks)
        if (mGas)
            if (mSpeed > -0.1)
                processGas(true);
            else
                processBreaks();
        else
            if (mSpeed > 0.1)
                processBreaks();
            else
                processGas(false);
    else
        processIdle();

    if (mHandBreaks)
        processHandBreaks();
}

void VehicleBase::setAimAngleSteering(const Ogre::Real angle)
{
    mAimAngle = angle * PI/180.0;
}

void VehicleBase::setCoeffForce(const Ogre::Real coeffForce)
{
    mCoeffForce = coeffForce/100.0f;
}

bool VehicleBase::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    this->updateVehicle(evt.timeSinceLastFrame);
    this->updateVehicleSteering(evt.timeSinceLastFrame);

    return true;
}

}
