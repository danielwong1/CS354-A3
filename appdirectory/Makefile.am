ACLOCAL_AMFLAGS= -I m4

noinst_HEADERS= BallGame.h Physics.h GameObject.h Wall.h Ball.h OgreMotionState.h BaseApplication.h Paddle.h Score.h BallScoreCallback.h BallPaddleCallback.h src/audio.h BallFloorCallback.h WallCallback.h Field.h Goal.h Arrow.h NetManager.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BallGame.cpp Physics.cpp GameObject.cpp Wall.cpp Ball.cpp OgreMotionState.cpp BaseApplication.cpp Paddle.cpp Score.cpp BallScoreCallback.cpp BallPaddleCallback.cpp src/audio.c BallFloorCallback.cpp WallCallback.cpp Field.cpp Goal.cpp Arrow.cpp NetManager.cpp

OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(SDL2_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDL_LIBS) $(SDL2_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -lSDL2 -lSDL_mixer -lSDL_net

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
