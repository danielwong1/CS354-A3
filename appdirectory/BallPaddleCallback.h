
#ifndef __BallPaddleCallback_h_
#define __BallPaddleCallback_h_

#include <btBulletCollisionCommon.h>

class BallGame;

struct BallPaddleCallback : public btCollisionWorld::ContactResultCallback {

	BallPaddleCallback(BallGame* bGame);
 public:
  btScalar addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1);

 private:
	BallGame* bGame;
};

#endif