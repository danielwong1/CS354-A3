#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"

class Paddle : public GameObject {
 public:
  Paddle(Ogre::SceneManager* sceneMgr, 
  Physics* simulator);

  void moveTo(const Ogre::Vector3 &position);
  void moveBy(const Ogre::Vector3 &distance);
  void rotateBy(const Ogre::Quaternion &rotation);
  void getRotation(btQuaternion& quaternion);
  btVector3 getPosition() const;
};

#endif