#include "Landscape.h"
#include "ITS.h"
#include "Physics.h"

namespace RAT
{

Landscape::Landscape()
    :mTerrain(0), mUseBinary(false)
{
        // ??????? ?????????, ???? ??? ??? ?? ???? ??????? (??. ????????? ????).
    if (Ogre::TerrainGlobalOptions::getSingletonPtr() == 0)
        new Ogre::TerrainGlobalOptions();
}

Landscape::~Landscape()
{
    if (mTerrain)
    {
            // ???? ???????? ????????, ?? ??????? ????? ??????
            // ?? ?????????? ???????? (??????? - ??????? ????????, ??? ???????)
        OGRE_DELETE mTerrain;

        Ogre::Node* terrainNode = ITS::getSceneManagerS()->getRootSceneNode()->getChild("Terrain");
        if (terrainNode)
            ITS::getSceneManagerS()->destroySceneNode((Ogre::SceneNode*)terrainNode);
    }
}

void Landscape::init(const LandscapeSettings& settings)
{
    mSettings = settings;
    Ogre::TerrainGlobalOptions::getSingleton().setMaxPixelError(1);
    Ogre::TerrainGlobalOptions::getSingleton().setCompositeMapDistance(2000);
    Ogre::TerrainGlobalOptions::getSingleton().setCompositeMapAmbient(ITS::getSceneManagerS()->getAmbientLight());

    if (settings.light)
    {
        Ogre::TerrainGlobalOptions::getSingleton().setLightMapDirection(settings.light->getDerivedDirection());
        Ogre::TerrainGlobalOptions::getSingleton().setCompositeMapDiffuse(settings.light->getDiffuseColour());
    }

    mTerrain = OGRE_NEW Ogre::Terrain(ITS::getSceneManagerS());

    mTerrain->setResourceGroup(settings.resourceGroup);
    if (mSettings.compiledTerrain.compare(""))
    {
        mUseBinary = true;
        if (Ogre::ResourceGroupManager::getSingleton().resourceExists(settings.resourceGroup, mSettings.compiledTerrain))
            return;

    }
    else
        mUseBinary = false;

        // ????????? RAW ????
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(settings.heightRawMap, settings.resourceGroup);

    size_t streamSize = stream.get()->size();
    if(streamSize != (settings.size + 1) * (settings.size + 1) * 4)
    {
        delete mTerrain;
        mTerrain = 0;
        OGRE_EXCEPT( Ogre::Exception::ERR_INTERNAL_ERROR, "Size of stream does not match terrainsize!", "TerrainPage" );
    }

    float* buffer = OGRE_ALLOC_T(float, streamSize, Ogre::MEMCATEGORY_GENERAL);
    stream->read(buffer, streamSize);

        // ????????? ???????
    mImportData.terrainSize = settings.size + 1;
    mImportData.worldSize = settings.size * settings.scaleXZ;
    mImportData.inputScale = settings.scaleY;
    mImportData.inputFloat = buffer;
    mImportData.deleteInputData = true;

    mImportData.minBatchSize = settings.minBatchSize;
    mImportData.maxBatchSize = settings.maxBatchSize;

}


void Landscape::addLayer(const Ogre::String& diffuseSpecularMap, const Ogre::String& normalHeightMap, const Ogre::String& blendMap, int worldSize)
{
    mBlendImages.push_back(blendMap);
    Ogre::Terrain::LayerInstance layer;
    layer.textureNames.push_back(diffuseSpecularMap);
    layer.textureNames.push_back(normalHeightMap);
    layer.worldSize = 600;
    mImportData.layerList.push_back(layer);
}

void Landscape::create()
{

    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mSettings.resourceGroup, mSettings.compiledTerrain))
    {
        mTerrain->load(mSettings.compiledTerrain);
    }
    else
    {
        mTerrain->prepare(mImportData);
        mTerrain->load();
        Ogre::Real multiplier = mTerrain->getLayerUVMultiplier(0);


            // ????????? ????? ??????????
        for(int j = 1; j < mTerrain->getLayerCount(); j++)
        {
            Ogre::TerrainLayerBlendMap *blendmap = mTerrain->getLayerBlendMap(j);
            Ogre::Image img;
            img.load(mBlendImages[j], mSettings.resourceGroup);
            int blendmapsize = mTerrain->getLayerBlendMapSize();
            if(img.getWidth() != blendmapsize)
                img.resize(blendmapsize, blendmapsize);

            float *ptr = blendmap->getBlendPointer();
            Ogre::uint8 *data = static_cast<Ogre::uint8*>(img.getPixelBox().data);

            for(int bp = 0;bp < blendmapsize * blendmapsize;bp++)
                ptr[bp] = static_cast<float>(data[bp]) / 255.0f;

            blendmap->dirty();
            blendmap->update();
        }

            // ??? ????????????? ?????????, ??? ?????????? ???????, ????? ???????? ??????????
        if (mUseBinary)
            ITS::GetRootS()->addFrameListener(this);
    }

        // ???? ????????? ???????? ???, ????? ????? ??? ? (0; 0; 0). ??????????, ????? ????? ???? ??? ? (0; 0; 0)
    mTerrain->setPosition(Ogre::Vector3(mSettings.scaleXZ*mSettings.size / 2.f, 0, mSettings.scaleXZ*mSettings.size / 2.f));

    mTerrain->freeTemporaryResources();
}

void Landscape::setupPhysics(float restitution, float friction)
{
    Ogre::Real * pDataConvert= new Ogre::Real[mTerrain->getSize() * mTerrain->getSize()];
    for (int i=0; i<mTerrain->getSize(); i++)
        memcpy(pDataConvert + mTerrain->getSize()*i,
            mTerrain->getHeightData() + mTerrain->getSize()*(mTerrain->getSize()-i-1),
            sizeof(float)*(mTerrain->getSize()) );

    float landSize = mTerrain->getSize();
    float maxHeight = mTerrain->getMaxHeight();
    mVector3 = Ogre::Vector3(mSettings.scaleXZ, 1.f, mSettings.scaleXZ);
    OgreBulletCollisions::HeightmapCollisionShape* shape = Physics::getSingleton().createHeightmapCollisionShape(landSize,landSize, pDataConvert, mVector3, maxHeight, false);


    OgreBulletDynamics::RigidBody* body = Physics::getSingleton().createRigidBody("landscape");

    Ogre::SceneNode* node = ITS::getSceneManagerS()->getRootSceneNode()->createChildSceneNode ("Terrain", mTerrain->getPosition());

    Ogre::Vector3 terrainShiftPos( mSettings.scaleXZ*(landSize - 1.f) / 2.f, (maxHeight - 1.f) / 2.f, mSettings.scaleXZ*(landSize-1.f) / 2.f);

    body->setStaticShape(node, shape, restitution, friction, terrainShiftPos);

    // ???? ????????????, ?????? ?? ?????. ???????? ?????? ?? ?????? ???? ?????.
    //delete pDataConvert;

}


Ogre::Terrain* Landscape::getTerrain()
{
    return mTerrain;
}

bool Landscape::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
        // ?????????, ??? ?????? ????????? ??????????
    if (!mTerrain->isDerivedDataUpdateInProgress())
    {
        mTerrain->save(mSettings.compiledTerrain);
        ITS::GetRootS()->removeFrameListener(this);
    }
    return true;
}

}
