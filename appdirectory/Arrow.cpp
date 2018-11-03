#include <btBulletCollisionCommon.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Arrow.h"
#include "Physics.h"
#include <OgreNode.h>
#include <iostream>


Arrow::Arrow(Ogre::SceneManager* sceneMgr) {

	this->name = "arrow";
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;

    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode("root");
    Ogre::SceneNode* childNode = rootNode->createChildSceneNode(this->name);
	geom = this->sceneMgr->createEntity(this->name, Ogre::SceneManager::PT_CUBE);

	childNode->attachObject(geom);
    childNode->scale(0.005f, 0.005f, 0.03f);
    childNode->translate(Ogre::Vector3(0, 0, -3.0f));

    geom->setCastShadows(false);
    geom->setMaterialName("Examples/Water0");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("Examples/Water0");
    mp.get()->setReceiveShadows(false);
}

btVector3 Arrow::getPosition() const {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);
    return worldTransform.getOrigin();
}


void Arrow::getRotation(btQuaternion& quaternion) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);
    quaternion = worldTransform.getRotation();
}


void Arrow::rotateArrowBy(const Ogre::Quaternion &rotation) {
    rootNode->rotate(rotation);
}

void Arrow::moveArrowTo(const Ogre::Vector3 &position) {
    rootNode->setPosition(position);
}