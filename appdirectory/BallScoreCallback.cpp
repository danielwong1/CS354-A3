#include <SDL2/SDL.h>
#include "BallScoreCallback.h"
#include "BallGame.h"
#include "Ball.h"
#include "Score.h"
#include "src/audio.h"
#include <iostream>

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallScoreCallback::BallScoreCallback(BallGame* pBallGame) : 
bGame(pBallGame){}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar BallScoreCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {

	// reset the score
	if(!bGame->mBall->colliding) {
		bGame->mBall->colliding = true;
        if(bGame->soundOn) { 
			playSound("sounds/ball_hit_wall.wav", SDL_MIX_MAXVOLUME);
		}
		bGame->collisionClock->reset();
		bGame->scoreObj->setScore(bGame->scoreObj->score + 1);
	}
}