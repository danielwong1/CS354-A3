
#include "GoalieWall.h"
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <btBulletCollisionCommon.h>
#include "Physics.h"

GoalieWall::GoalieWall(Ogre::String name, Ogre::SceneManager* sceneMgr,
    Physics* simulator) {
    this->name = name;
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;

    Ogre::Vector3 direction = Ogre::Vector3::UNIT_Z;
    // //TODO: The wall is hard coded to faced the Z direction; flip it when done
    Ogre::MovablePlane plane(direction, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        plane, 21, 10, 1, 1, true, 1, 1, 1, 
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
    // tr.setOrigin(-23.5f * btDir);
    shape = new btBoxShape(btVector3(10, 5, 0.1f));
    mass = 0.0;
    motionState = new OgreMotionState(tr, rootNode);
    body = new btRigidBody(mass, motionState, shape);
    body->setUserPointer((void*) this);
    body->setRestitution(1.0f);
    body->setFriction(0);
    simulator->dynamicsWorld->addRigidBody(body, GOAL_WALL, GOAL_WALL | ETC);

    this->moveBy(Ogre::Vector3(0, 4.5f, -23.5f));
}