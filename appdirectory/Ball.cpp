#include <btBulletCollisionCommon.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include "Ball.h"
#include "Physics.h"


Ball::Ball(Ogre::String name, 
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator) {

	this->name = name;
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;
    colliding = false;
    f_colliding = false;
    floorBounces = 0;
    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
	geom = this->sceneMgr->createEntity(name, "sphere.mesh");
	rootNode->attachObject(geom);
	rootNode->scale(0.005f, 0.005f, 0.005f);
    	
    geom->setCastShadows(true);
    geom->setMaterialName("OceanHLSL_GLSL");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("OceanHLSL_GLSL");
    mp.get()->setReceiveShadows(false);

	shape = new btSphereShape(btScalar(.5f));
	mass = 0.05f;
    shape->calculateLocalInertia(mass, inertia);
	motionState = new OgreMotionState(tr, rootNode);
        
	body = new btRigidBody(mass, motionState, shape, inertia);
    body->setUserPointer((void*) this);
    body->setRestitution(0.95f);
    body->setFriction(0);
    simulator->dynamicsWorld->addRigidBody(body);
    simulator->gameObjects.push_back(this);
}