#include "Field.h"
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <btBulletCollisionCommon.h>
#include "Physics.h"

Field::Field( 
  Ogre::SceneManager* sceneMgr, 
  Physics* simulator
) {
  this->name = name;
  this->sceneMgr = sceneMgr;
  this->simulator = simulator;
  Ogre::Vector3 direction(0, 1, 0);

  Ogre::MovablePlane plane(direction, 0.0);
  Ogre::MeshManager::getSingleton().createPlane(
      name + "mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
      plane, 100, 100, 5, 5, true, 1, 1, 1,
      direction.perpendicular()
  );

  rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
  geom = this->sceneMgr->createEntity(name, name + "mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  rootNode->attachObject(geom);

  geom->setMaterialName("assignment3/grass");
  Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("assignment3/grass");
  mp.get()->setReceiveShadows(true);
  geom->setCastShadows(false);

  tr.setOrigin(btVector3(0.0, 0.0, 0.0));
  shape = new btBoxShape(btVector3(100.0f, 0.2f, 100.0f));
  mass = 0.0;
  motionState = new OgreMotionState(tr, rootNode);
  body = new btRigidBody(mass, motionState, shape);
  body->setUserPointer((void*) this);
  body->setRestitution(0.08f);
  body->setFriction(0);
  simulator->dynamicsWorld->addRigidBody(body);
}