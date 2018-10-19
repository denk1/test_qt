#include "Physics.h"
#include "MyTestApp.h"
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>

template<> RAT::Physics* Ogre::Singleton<RAT::Physics>::msSingleton = 0;

namespace RAT
{

Physics::Physics(const Ogre::AxisAlignedBox& bounds, bool debug, const Ogre::Vector3& gravityVector)
	:mDebugDrawer(0), mNumEntitiesInstanced(0)
{
    mWorld = new OgreBulletDynamics::DynamicsWorld(MyTestApp::getSceneManagerS(), bounds, gravityVector);
    mWorld->setShowDebugContactPoints(true);
    if(debug)
		{
		// add Debug info display tool
		mDebugDrawer = new OgreBulletCollisions::DebugDrawer();
		mDebugDrawer->setDrawWireframe(true);   // we want to see the Bullet containers
		mWorld->setDebugDrawer(mDebugDrawer);
		mWorld->setShowDebugShapes(true);      // enable it if you want to see the Bullet containers
        Ogre::SceneNode *node = MyTestApp::getSceneManagerS()->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
		node->attachObject(static_cast <Ogre::SimpleRenderable *> (mDebugDrawer));
	}

    MyTestApp::GetRootS()->addFrameListener(this);
}

Physics::~Physics()
{
    MyTestApp::GetRootS()->removeFrameListener(this);

	free();

	if(mDebugDrawer)
	{
		delete mDebugDrawer;
		mDebugDrawer = 0;
		mWorld->setDebugDrawer(0);
		
	}
	
	delete mWorld;

}

void Physics::free()
{
	// OgreBullet physic delete - RigidBodies
	std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
	while (mBodies.end() != itBody)
	{   
		delete *itBody;
		++itBody;
	}   
	// OgreBullet physic delete - Shapes
	std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
	while (mShapes.end() != itShape)
	{   
		delete *itShape;
		++itShape;
	}
	mBodies.clear();
	mShapes.clear();
}

void Physics::destroyShape(OgreBulletCollisions::CollisionShape* shape)
{
	std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
	while (mShapes.end() != itShape)
	{
		if ((*itShape) == shape)
		{
			delete *itShape;
			mShapes.erase(itShape);
			
			return;
		}
		++itShape;
	}
}

void Physics::destroyRigidBody(OgreBulletDynamics::RigidBody* body)
{
	std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
	while (mBodies.end() != itBody)
	{   
		if ((*itBody) == body)
		{
			delete *itBody;
			mBodies.erase(itBody);
			
			return;
		}
		++itBody;
	}   
}

//OgreBulletDynamics::DynamicsWorld* Physics::getWorld()
//{
//	return mWorld;
//}


bool Physics::frameStarted(const Ogre::FrameEvent& evt)
{
		// update Bullet Physics animation
	//mWorld->stepSimulation(2.f*evt.timeSinceLastEvent, 5); 
	//mWorld->stepSimulation(evt.timeSinceLastFrame, 4);  
	mWorld->stepSimulation(2.f*evt.timeSinceLastFrame, 4);  
	TPCamera::getSingleton().update();
	return true;
}

bool Physics::frameEnded(const Ogre::FrameEvent& evt)
{
		// update Bullet Physics animation
	//mWorld->stepSimulation(2.f*evt.timeSinceLastEvent, 4);
	TPCamera::getSingleton().update();

	return true;
}

OgreBulletCollisions::CollisionShape* Physics::createCollisionShapeFromMesh(Ogre::Entity *entity, const Ogre::Matrix4& matrix)
{
	OgreBulletCollisions::StaticMeshToShapeConverter converter(entity, matrix);
	OgreBulletCollisions::CollisionShape* shape = (OgreBulletCollisions::CollisionShape*)converter.createTrimesh();

	mShapes.push_back(shape);
	return shape;
}


OgreBulletCollisions::BoxCollisionShape* Physics::createBoxCollisionShape(const Ogre::Vector3  &boxBounds)
{
	OgreBulletCollisions::BoxCollisionShape* shape = new OgreBulletCollisions::BoxCollisionShape(boxBounds);
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::SphereCollisionShape* Physics::createSphereCollisionShape(const float radius)
{
	OgreBulletCollisions::SphereCollisionShape* shape = new OgreBulletCollisions::SphereCollisionShape(radius);
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::CapsuleCollisionShape* Physics::createCapsuleCollisionShape(const Ogre::Real radius, const Ogre::Real height, const Ogre::Vector3 &axe)
{
	OgreBulletCollisions::CapsuleCollisionShape* shape = new OgreBulletCollisions::CapsuleCollisionShape(radius, height, axe);
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::CylinderCollisionShape* Physics::createCylinderCollisionShape(const Ogre::Vector3  &halfExtents, const Ogre::Vector3 &axe)
{
	OgreBulletCollisions::CylinderCollisionShape* shape = new OgreBulletCollisions::CylinderCollisionShape(halfExtents, axe);
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::ConeCollisionShape* Physics::createConeCollisionShape(Ogre::Real radius, Ogre::Real height, const Ogre::Vector3 &axe)
{
	OgreBulletCollisions::ConeCollisionShape* shape = new OgreBulletCollisions::ConeCollisionShape(radius, height, axe);
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::MultiSphereCollisionShape* Physics::createMultiSphereCollisionShape(
            const Ogre::Vector3& inertiaHalfExtents,
            const Ogre::Vector3* positions,
            const Ogre::Real* radi,
            int numSpheres)
{
	OgreBulletCollisions::MultiSphereCollisionShape* shape = new OgreBulletCollisions::MultiSphereCollisionShape(inertiaHalfExtents, positions, radi, numSpheres);
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::CompoundCollisionShape* Physics::createCompoundCollisionShape()
{
	OgreBulletCollisions::CompoundCollisionShape* shape = new OgreBulletCollisions::CompoundCollisionShape();
	mShapes.push_back(shape);
	return shape;
}

OgreBulletCollisions::HeightmapCollisionShape* Physics::createHeightmapCollisionShape(int width, int length, Ogre::Real* heightData, Ogre::Vector3& scale, int maxHeight, bool flip)
{
    //OgreBulletCollisions::HeightmapCollisionShape* shape = new OgreBulletCollisions::HeightmapCollisionShape(width, length, scale, maxHeight, heightData, flip);
    OgreBulletCollisions::HeightmapCollisionShape* shape = new OgreBulletCollisions::HeightmapCollisionShape(width, length, scale, heightData, flip);
    mShapes.push_back(shape);
	
    return shape;
}

OgreBulletDynamics::RigidBody* Physics::createRigidBody(const Ogre::String &name, 
													const short collisionGroup,
													const short collisionMask)
{
	OgreBulletDynamics::RigidBody* body = new OgreBulletDynamics::RigidBody(name + "_" + Ogre::StringConverter::toString(mNumEntitiesInstanced), 
														mWorld, collisionGroup, collisionMask);

	mBodies.push_back(body);  
	++mNumEntitiesInstanced;

    return body;
}

}

//void Physics::addRigidBody(OgreBulletDynamics::RigidBody* rigidBody, 
//						const short collisionGroup, const short collisionMask)
//{
//	mWorld->addRigidBody(rigidBody, collisionGroup, collisionMask);
//	++mNumEntitiesInstanced;
//}
