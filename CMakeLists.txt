cmake_minimum_required(VERSION 2.8)

# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)


# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
include_directories(/usr/local/include/OGRE/RTShaderSystem)
INCLUDE_DIRECTORIES(/usr/local/include/OgreBullet/Dynamics)
INCLUDE_DIRECTORIES(/usr/local/include/OgreBullet/Collisions)
INCLUDE_DIRECTORIES(/usr/local/include/bullet)
link_directories(/usr/local/lib/)
LINK_DIRECTORIES(/home/den/Downloads/ogre-1.11.3/build/freetype-2.9/objs)
LINK_DIRECTORIES(/home/den/Downloads/build-ogre-1.11.2-Desktop-Release/Dependencies/lib/)
project(its)
add_executable(${PROJECT_NAME} main.cpp
                               TPCamera.cpp
                               ITS.cpp
                               GameState.cpp
                               Physics.cpp
                               GSStart.cpp
                               Map.cpp
                               MapLoader.cpp
                               GSMap.cpp
                               BBLoader.cpp
                               BonusBuilding.cpp
                               BonusBuildingBase.cpp
                               Config.cpp
                               Landscape.cpp
                               Tower.cpp
                               TowerBase.cpp
                               TowerLoader.cpp
                               Weapon.cpp
                               WeaponFactory.cpp
                               WeaponHolder.cpp
                               WeaponLoader.cpp
                               Vehicle.cpp
                               VehicleBase.cpp
                               VehicleLoader.cpp
                               VehiclePrototype.cpp
                               SceneLoader.cpp
                               DotSceneLoader.cpp
                               PhysicsShapesLoader.cpp
                               WGun.cpp
                               WRocketLauncher.cpp
                               )
target_link_libraries(its OgreMain
                              OgreBites
                              OgreTerrain
                              freetype
                              Bullet3Common
                              Bullet3Dynamics
                              BulletCollision
                              BulletDynamics
                              LinearMath
                              boost_system
                              OgreBulletDynamics
                              OgreBulletCollisions
                              tinyxml

                             )
