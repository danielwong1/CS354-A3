#ifndef __OgreMotionState_h_
#define __OgreMotionState_h_

#include <btBulletDynamicsCommon.h>
#include <OgreRoot.h>

class OgreMotionState : public btMotionState { 
    protected: 
    Ogre::SceneNode* mObj; 
    btTransform mPos; 

    public:
    OgreMotionState(const btTransform &initialpos, Ogre::SceneNode* node);
    virtual ~OgreMotionState();
    void setNode(Ogre::SceneNode* node);
    virtual void getWorldTransform(btTransform &worldTrans) const;
    virtual void setWorldTransform(const btTransform &worldTrans);
};

#endif