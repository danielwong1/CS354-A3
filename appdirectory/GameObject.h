#ifndef __GameObject_h_
#define __GameObject_h_

#include <OgreRoot.h>
#include <btBulletDynamicsCommon.h>
#include "OgreMotionState.h"

class Physics;

class GameObject {
    public:
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Physics* simulator;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    btCollisionShape* shape;
    btScalar mass;
    btRigidBody* body;
    btTransform tr;
    btVector3 inertia;
    OgreMotionState* motionState;

    GameObject(Ogre::String name,
    Ogre::SceneManager* sceneMgr,
    Physics* simulator,
    Ogre::SceneNode* rootNode,
    Ogre::Entity* geom,
    btCollisionShape* shape,
    btScalar mass,
    btRigidBody* body,
    btTransform tr,
    btVector3 inertia,
    OgreMotionState* motionState);

    GameObject();
};

#endif