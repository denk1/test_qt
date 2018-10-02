#include <Ogre.h>
#include "Physics.h"
#include "tinyxml.h"
#include <map>
#include <algorithm>

namespace RAT
{
	class PhysicsShapesLoader
	{
		typedef OgreBulletCollisions::CollisionShape* (RAT::PhysicsShapesLoader::*CreateCollisionShapesPtr)(TiXmlElement* xmlNode, Ogre::Vector3& position);
		typedef std::map<Ogre::String, CreateCollisionShapesPtr> MapCollisionShapesType;
	public :
		PhysicsShapesLoader();
		~PhysicsShapesLoader();
		OgreBulletCollisions::CompoundCollisionShape* parsePhysics(TiXmlElement* xmlNode);
	private:
		MapCollisionShapesType mCollisionShapes;
		OgreBulletCollisions::CollisionShape* parseCollisionShapesBox(TiXmlElement* xmlNode, Ogre::Vector3&);
		OgreBulletCollisions::CollisionShape* parseCollisionShapesSphere(TiXmlElement* xmlNode, Ogre::Vector3&);
		OgreBulletCollisions::CollisionShape* parseCollisionShapesCapsule(TiXmlElement* xmlNode, Ogre::Vector3&);
		OgreBulletCollisions::CollisionShape* parseCollisionShapesCone(TiXmlElement* xmlNode, Ogre::Vector3&);
		OgreBulletCollisions::CollisionShape* parseCollisionShapesCylinder(TiXmlElement* xmlNode, Ogre::Vector3&);
		OgreBulletCollisions::CollisionShape* parseCollisionShapesMultiSphere(TiXmlElement* xmlNode, Ogre::Vector3&);
		OgreBulletCollisions::CollisionShape* parseCollisionShapesCompound(TiXmlElement* xmlNode, Ogre::Vector3& position);
		OgreBulletCollisions::CompoundCollisionShape* getPhysics(TiXmlElement* xmlNode);
		void getXmlTagVectorAttribute(TiXmlElement* xmlNode, Ogre::Vector3& outVector);
		Ogre::Real getXmlRealAttribute(TiXmlElement* xmlNode, const char*);
	};
}