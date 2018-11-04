#include <btBulletCollisionCommon.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Paddle.h"
#include "Physics.h"
#include <iostream>


Paddle::Paddle(Ogre::SceneManager* sceneMgr, 
    Physics* simulator) {

	this->name = "paddle";
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;

    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
	geom = this->sceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
	rootNode->attachObject(geom);
    rootNode->scale(0.04f, 0.04f, 0.01f);

    geom->setCastShadows(false);
    geom->setMaterialName("paddle/Material");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("paddle/Material");
    mp.get()->setReceiveShadows(false);

	shape = new btBoxShape(btVector3(2.0f, 2.0f, 0.5f));
	mass = 0.0f;
	motionState = new OgreMotionState(tr, rootNode);

	body = new btRigidBody(mass, motionState, shape, inertia);

    body->setRestitution(1.0f);
    body->setFriction(0);
    simulator->dynamicsWorld->addRigidBody(body);
    simulator->gameObjects.push_back(this);

    // body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    // body->setActivationState(DISABLE_DEACTIVATION);
    moveBy(Ogre::Vector3(0.0f, 2.0f, 8.0f));    
}


void Paddle::getRotation(btQuaternion& quaternion) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);
    quaternion = worldTransform.getRotation();
}
