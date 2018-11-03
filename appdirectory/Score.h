#ifndef __Score_h_
#define __Score_h_

#include <CEGUI/CEGUI.h>

class Score
{
    public:
        int hostScore;
        int clientScore;
        CEGUI::Window *scoreWindow;

        Score();
        void incrementClientScore(void); 
        void incrementHostScore(void);
        void setScore(void);

};

#endif
