#include "Wall.h"
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <btBulletCollisionCommon.h>
#include "Physics.h"
#include "Field.h"

Wall::Wall(Ogre::String name, 
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator,
    Ogre::Vector3 direction) {
    this->name = name;
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;
    
    Ogre::MovablePlane plane(direction, -Field::SIZE/2);
    Ogre::MeshManager::getSingleton().createPlane(
        name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        plane, Field::SIZE, Field::SIZE, Field::SIZE, Field::SIZE, true, 1, 1, 1, 
        direction.perpendicular()
    );

    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
    geom = this->sceneMgr->createEntity(name);
    rootNode->attachObject(geom);

    geom->setMaterialName("invisible/Material");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("invisible/Material");
    mp.get()->setReceiveShadows(true);
    geom->setCastShadows(false);

    btVector3 btDir = btVector3(direction.x, direction.y, direction.z);
    tr.setOrigin(-Field::SIZE * btDir);
    shape = new btBoxShape(btVector3(Field::SIZE/2, Field::SIZE/2, Field::SIZE/2));
    mass = 0.0;
    motionState = new OgreMotionState(tr, rootNode);
    body = new btRigidBody(mass, motionState, shape);
    body->setUserPointer((void*) this);
    body->setRestitution(1.0f);
    body->setFriction(0);
    simulator->dynamicsWorld->addRigidBody(body);
}