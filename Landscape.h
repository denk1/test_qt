#ifndef Landscape_H
#define Landscape_H

#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

namespace RAT
{

// Настройки импорта
struct LandscapeSettings
{
		// Имя карты высот. Карта должна быть в формате RAW.
	Ogre::String heightRawMap;
		// Размер карты высот по ширине и длинне (size x size).
	int size;
		// Масштаб по ширине и длине
	Ogre::Real scaleXZ;
		// Масштаб по высоте
	Ogre::Real scaleY;
		// Источник света, освещающий ландшафт.
	Ogre::Light* light;
		// Группа ресурсов, в которой хранится heightRawMap
	Ogre::String resourceGroup;

	int minBatchSize;

	int maxBatchSize;

		// Файл в котором хранится скомпилированный ландшафт, если файла нет, 
		// то ландшафт компилируется и сохраняется в этот файл. Ускоряет загрузку. Используется, если compiledTerrain != ""
	Ogre::String compiledTerrain;

        LandscapeSettings()
	: heightRawMap("terrain.raw")
	, size(256)
	, scaleXZ(1.0f)
	, scaleY(1.0f)
	, light(0)
	, resourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
	, minBatchSize(33)
	, maxBatchSize (65)
	, compiledTerrain("")
	{
	}
};



	// Ландшафт, с использованием карты высот в формате .raw с точкой формата float ( 4 байта )
class Landscape : public Ogre::FrameListener
{
public:

	
	Landscape();
	~Landscape();

	/** Инициализация
		@param settings - Настройки импорта.
	*/
	void init(const LandscapeSettings& settings);

	/** Добавляет слой к ландшафту. Максимальное количество слоев - 4.
		@param diffuseSpecularMap - Имя текстуры, содержащей в RGB - diffuse составляющую, а в Альфа канале - Specular.
		@param normalHeightMap - Имя текстуры, содержащей карту высот данного слоя.
		@param blendMap - Имя текстуры[оттенок серого] содержащей параметр блендинга с другими слоями.
		@param worldSize - Размер на который растягивается текстура
	*/
	void addLayer(const Ogre::String& diffuseSpecularMap, const Ogre::String& normalHeightMap, const Ogre::String& blendMap, int worldSize); 

	/** Создает ландшафт. */
	void create(); 

	/** Создает физику ландшафта */
	void setupPhysics(float restitution = 0.6f, float friction = 0.6f); 

	/** Возвращает указатель на базый ландшафт */
	Ogre::Terrain* getTerrain();

protected:

	protected:
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	Ogre::Terrain* mTerrain;
	Ogre::Terrain::ImportData	mImportData;
	LandscapeSettings			mSettings;
	std::vector<Ogre::String>	mBlendImages;
	bool						mUseBinary;

        Ogre::Vector3 mVector3;
    Ogre::TerrainGroup* mTerrainGroup;

};

}

#endif
