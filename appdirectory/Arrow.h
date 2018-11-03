#ifndef __Arrow_h_
#define __Arrow_h_

#include "GameObject.h"

class Arrow : public GameObject {
 public:
  Arrow(Ogre::SceneManager* sceneMgr);

  void getRotation(btQuaternion& quaternion);
  btVector3 getPosition() const;
  void rotateArrowBy(const Ogre::Quaternion &rotation);
  void moveArrowTo(const Ogre::Vector3 &position);
};

#endif