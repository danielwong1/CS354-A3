#include "GameObject.h"

GameObject::GameObject(
    Ogre::String name,
    Ogre::SceneManager* sceneMgr,
    Physics* simulator,
    Ogre::SceneNode* rootNode,
    Ogre::Entity* geom,
    btCollisionShape* shape,
    btScalar mass,
    btRigidBody* body,
    btTransform tr,
    btVector3 inertia,
    OgreMotionState* motionState):
    
    name(name), 
    sceneMgr(sceneMgr), 
    simulator(simulator),
    rootNode(rootNode),
    geom(geom),
    shape(shape),
    mass(mass),
    body(body),
    tr(tr),
    inertia(inertia), 
    motionState(motionState) {
}

GameObject::GameObject() : name(""), 
    sceneMgr(NULL), 
    simulator(NULL),
    rootNode(NULL),
    geom(NULL),
    shape(NULL),
    mass(BT_ONE),
    body(NULL),
    tr(btTransform::getIdentity()),
    inertia(btVector3(0.0f, 0.0f, 0.0f)),
    motionState(NULL) { 
}