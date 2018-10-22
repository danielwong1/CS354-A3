#include "OgreMotionState.h"

OgreMotionState::OgreMotionState(const btTransform &initialpos, Ogre::SceneNode* node) { 
    mObj = node; 
    mPos = initialpos; 
} 

OgreMotionState::~OgreMotionState() {} 
//Provides flexibility in terms of object visibility 
void OgreMotionState::setNode(Ogre::SceneNode* node) { 
    mObj = node; 
} 

void OgreMotionState::getWorldTransform(btTransform &worldTrans) const { 
    worldTrans = mPos; 
} 

void OgreMotionState::setWorldTransform(const btTransform &worldTrans) { 
    if (mObj == NULL) 
        return; // silently return before we set a node 
    btQuaternion rot = worldTrans.getRotation(); 
    mObj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z()); 
    btVector3 pos = worldTrans.getOrigin();
    mObj->setPosition(pos.x(), pos.y(), pos.z()); 
    mPos = worldTrans;
}
