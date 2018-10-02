#include "SceneLoader.h"
#include "Physics.h"
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>

namespace RAT
{

void SceneLoader::entityLoaded(Ogre::Entity *entity, Ogre::SceneNode* node)
{
	Ogre::Matrix4 matrix = Ogre::Matrix4::IDENTITY;
	matrix.setScale(node->getScale());
	
	OgreBulletCollisions::CollisionShape* shape = Physics::getSingleton().createCollisionShapeFromMesh(entity, matrix);
	
	OgreBulletDynamics::RigidBody* body = Physics::getSingleton().createRigidBody(entity->getName());
	body->setStaticShape(node, shape, 1.0f, 0.6f, node->_getDerivedPosition(), node->_getDerivedOrientation());	
}

}
