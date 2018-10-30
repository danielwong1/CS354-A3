#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"

class Paddle : public GameObject {
 public:
  Paddle(Ogre::SceneManager* sceneMgr, 
  Physics* simulator);

  void getRotation(btQuaternion& quaternion);
};

#endif