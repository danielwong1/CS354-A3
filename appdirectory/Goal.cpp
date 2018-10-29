#include <btBulletCollisionCommon.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include "Goal.h"
#include "Physics.h"


Goal::Goal(
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator) {

	this->name = "Goal";
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;
    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
	geom = this->sceneMgr->createEntity(name, "gate.mesh");
	rootNode->attachObject(geom);
	rootNode->scale(0.05f, 0.05f, 0.075f);

    geom->setCastShadows(true);
    geom->setMaterialName("gate/Material");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("gate/Material");
    mp.get()->setReceiveShadows(false);

	shape = new btBoxShape(btVector3(15, 15, 15));
	mass = 0.05f;
    shape->calculateLocalInertia(mass, inertia);
	motionState = new OgreMotionState(tr, rootNode);
        
	body = new btRigidBody(mass, motionState, shape, inertia);
    body->setUserPointer((void*) this);
    body->setRestitution(0.95f);
    simulator->dynamicsWorld->addRigidBody(body);
    simulator->gameObjects.push_back(this);

    moveBy(Ogre::Vector3(0, 4.5, -25));
    rotateBy(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90)), Ogre::Vector3::UNIT_Y));
}