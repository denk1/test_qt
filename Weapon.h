#ifndef Weapon_H
#define Weapon_H

#include <Ogre.h>

namespace RAT
{

	// Описание оружия.
struct WeaponDescription
{
	Ogre::String	mMeshFile;	// Модель
	Ogre::String	mType;		// Тип оружия
	Ogre::Real		mMinAngle;	// Максимальный угол вращения против часовой стрелки
	Ogre::Real		mMaxAngle;	// Максимальный угол вращения про часовой стрелки
	Ogre::Real		mRotateSpeed;	// Скорость вращения
	Ogre::Vector3	mPosition;		// Смещение относительно крепления
	Ogre::Real		mBaseDamage;	// Базовый урон
	Ogre::Real		mReloadTime;	// Время перезарядки
	
	WeaponDescription()
		: mMeshFile("")
		, mType("")
		, mMinAngle(0.f)
		, mMaxAngle(0.f)
		, mRotateSpeed(0.f)
		, mPosition()
		, mBaseDamage(0.f)
		, mReloadTime(0.f)
	{
	}
};

	// Класс оружия. Представляет как оружие башен, так и машин.
class Weapon
{
public:

	Weapon(const WeaponDescription& desc);
	virtual ~Weapon(){}

	const WeaponDescription& getDescription();

	//const Ogre::Vector3& getDirection();
	// Плавно поварачивает пушку с текущего направления на новое, учитывая парамтр "скорость вращения"  
	//virtual void rotateTo(const Ogre::Vector3& direction);

	// Возвращает основной узел оружия
	virtual Ogre::SceneNode* getSceneNode();

	virtual void setOrientation(const Ogre::Radian angle);

	// Создает оружие и помещает на сцену.
	virtual void create(Ogre::SceneNode* parentNode) = 0;

	// Начать стрелять
	virtual void startShoot(const Ogre::Real dTime) = 0;
	// Закончить стрелять
	virtual void stopShoot(const Ogre::Real dTime) = 0;
	// Обновление каждый кадр
	virtual void update(const Ogre::Real dTime) = 0;

	
protected:

	WeaponDescription mDescription;

	Ogre::SceneNode* mSceneNode;
};

}

#endif