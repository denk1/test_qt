#ifndef Physics_H
#define Physics_H

#include <Ogre.h>
#include <OgreSingleton.h>
#include <OgreBulletDynamicsRigidBody.h>

#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <Shapes/OgreBulletCollisionsSphereShape.h>
#include <Shapes/OgreBulletCollisionsCapsuleShape.h>
#include <Shapes/OgreBulletCollisionsCylinderShape.h>
#include <Shapes/OgreBulletCollisionsConeShape.h>
#include <Shapes/OgreBulletCollisionsMultiSphereShape.h>
#include <Shapes/OgreBulletCollisionsTerrainShape.h>
#include <Shapes/OgreBulletCollisionsCompoundShape.h>

namespace RAT
{
	
class Physics : public Ogre::Singleton<Physics>, public Ogre::FrameListener
{
public:

    Physics(const Ogre::AxisAlignedBox& bounds, bool debug = false, const Ogre::Vector3& gravityVector = Ogre::Vector3 (0,  -9.81f,  0));
	~Physics();

		// Удаляет все созданные объекты
	void free();

	void destroyShape(OgreBulletCollisions::CollisionShape* shape);
	void destroyRigidBody(OgreBulletDynamics::RigidBody* body);

	OgreBulletCollisions::BoxCollisionShape* createBoxCollisionShape(const Ogre::Vector3  &boxBounds);
	OgreBulletCollisions::SphereCollisionShape* createSphereCollisionShape(const float radius);
	OgreBulletCollisions::CapsuleCollisionShape* createCapsuleCollisionShape(const Ogre::Real radius, const Ogre::Real height, const Ogre::Vector3 &axe);
	OgreBulletCollisions::CylinderCollisionShape* createCylinderCollisionShape(const Ogre::Vector3  &halfExtents, const Ogre::Vector3 &axe);
	OgreBulletCollisions::ConeCollisionShape* createConeCollisionShape(Ogre::Real radius, Ogre::Real height, const Ogre::Vector3 &axe);
	OgreBulletCollisions::MultiSphereCollisionShape* createMultiSphereCollisionShape( const Ogre::Vector3& inertiaHalfExtents,
																					  const Ogre::Vector3* positions,
																					  const Ogre::Real* radius,
																					  int numSpheres);
	OgreBulletCollisions::CompoundCollisionShape* createCompoundCollisionShape();
	OgreBulletCollisions::HeightmapCollisionShape* createHeightmapCollisionShape(int width, int length, Ogre::Real* heightData, 
																					Ogre::Vector3& scale, int maxHeight, bool flip = false);
	
	OgreBulletCollisions::CollisionShape* createCollisionShapeFromMesh(Ogre::Entity *entity, const Ogre::Matrix4& matrix = Ogre::Matrix4::IDENTITY);
	
	OgreBulletDynamics::RigidBody* createRigidBody(const Ogre::String &name = "RigidBody", 
													  const short collisionGroup = 0,
													  const short collisionMask = 0);

	

	/*void addRigidBody(OgreBulletDynamics::RigidBody* rigidBody, 
						const short collisionGroup = 0, const short collisionMask = 0);*/
	
	OgreBulletDynamics::DynamicsWorld* getWorld(){return mWorld;}



protected:

	// Ogre::FrameListener
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	OgreBulletDynamics::DynamicsWorld *mWorld;   // OgreBullet World
	OgreBulletCollisions::DebugDrawer *mDebugDrawer;
	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	int mNumEntitiesInstanced;
};

}

#endif
