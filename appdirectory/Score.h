#ifndef __Score_h_
#define __Score_h_

#include <CEGUI/CEGUI.h>

class Score
{
    public:
        int score;
        CEGUI::Window *scoreWindow;

        Score();
        void setScore(int count);
};

#endif
