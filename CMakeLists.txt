cmake_minimum_required(VERSION 2.8)

# specify which version and components you need
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/home/den/Downloads/build-ogre-1.11.2-Desktop-Release/cmake")
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)
#find_package(OIS)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/home/den/Projects/OIS/build/CMakeFiles")
#find_package(ois CONFIG REQUIRED)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
include_directories(/usr/local/include/OGRE/RTShaderSystem)
INCLUDE_DIRECTORIES(/usr/local/include/cegui-0)
#INCLUDE_DIRECTORIES(/usr/local/include/cegui-0/CEGUI)
INCLUDE_DIRECTORIES(/usr/local/include/ois)
link_directories(/usr/local/lib/)
LINK_DIRECTORIES(/home/den/Downloads/build-ogre-1.11.2-Desktop-Debug/Dependencies/lib)
LINK_DIRECTORIES(/home/den/Downloads/build-ogre-1.11.2-Desktop-Debug/freetype-2.9/objs)
project(test_qt)
add_executable(${PROJECT_NAME} main.cpp
                               TPCamera.cpp
                               MyTestApp.cpp
                               PlayerInterface.cpp
                               )
target_link_libraries(test_qt OgreMain
                              OgreBites
                              freetype
                             )
