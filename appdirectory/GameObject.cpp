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

void GameObject::moveBy(const Ogre::Vector3 &distance) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);

    btVector3 origin = worldTransform.getOrigin();
    worldTransform.setOrigin(origin + btVector3(distance.x, distance.y, distance.z));
    motionState->setWorldTransform(worldTransform);
}

void GameObject::moveTo(const Ogre::Vector3 &position) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);

    worldTransform.setOrigin(btVector3(position.x, position.y, position.z));
    motionState->setWorldTransform(worldTransform);
}

void GameObject::rotateBy(const Ogre::Quaternion &rotation) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);

    Ogre::Radian angle;
    Ogre::Vector3 axis;
    rotation.ToAngleAxis(angle, axis);

    btQuaternion bulletRotation(btVector3(axis.x, axis.y, axis.z), btScalar(angle.valueRadians()));
    btQuaternion currentRotation = worldTransform.getRotation();
    worldTransform.setRotation(currentRotation * bulletRotation);
    motionState->setWorldTransform(worldTransform);
}
