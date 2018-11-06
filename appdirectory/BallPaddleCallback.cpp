
#include <SDL2/SDL.h>
#include "BallPaddleCallback.h"
#include "BallGame.h"
#include "Ball.h"
#include "Paddle.h"
#include "src/audio.h"

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallPaddleCallback::BallPaddleCallback(BallGame* pBallGame) : bGame(pBallGame){}

/**
 * Callback for whenever the ball collides with the Paddle
 *
 */
btScalar BallPaddleCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {


	
	if(bGame->soundOn) {
		playSound("sounds/ball_hit_paddle.wav", SDL_MIX_MAXVOLUME / 10);
	}
}