#ifndef SceneLoader_H
#define SceneLoader_H

#include "DotSceneLoader.h"

namespace RAT
{

class SceneLoader : public Ogre::DotSceneLoader
{
public:

protected:

	void entityLoaded(Ogre::Entity *entity, Ogre::SceneNode* node);

};

}

#endif