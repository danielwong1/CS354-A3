#include <SDL2/SDL.h>
#include "WallCallback.h"
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
WallCallback::WallCallback(BallGame* pBallGame) : 
bGame(pBallGame){}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar WallCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {

	// reset the score
    if(bGame->soundOn) { 
		playSound("sounds/ball_hit_wall.wav", SDL_MIX_MAXVOLUME/15);
	}
}