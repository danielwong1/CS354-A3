#ifndef __BallScoreCallback_h_
#define __BallScoreCallback_h_

#include <btBulletCollisionCommon.h>

class BallGame;
class NetManager;

struct BallScoreCallback : public btCollisionWorld::ContactResultCallback {

	BallScoreCallback(BallGame* ballGame, NetManager* network);
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
     NetManager* network;
};

#endif