#include "BallFloorCallback.h"
#include "BallGame.h"
#include "Ball.h"
#include "Score.h"
#include "src/audio.h"
/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallFloorCallback::BallFloorCallback(BallGame* ballGame) : 
bGame(ballGame){}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar BallFloorCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {
	
	// reset the score
	if(!bGame->mBall->f_colliding) {
		bGame->mBall->f_colliding = true;
        bGame->mBall->floorBounces++;
		bGame->collisionClock->reset();
	}

	if(bGame->soundOn) { 
		playSound("sounds/ball_hit_wall.wav", SDL_MIX_MAXVOLUME/15);
	}
}