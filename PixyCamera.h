//
// Created by TingMiao on 20/11/2016.
//

#ifndef HELLO_PIXYCAMERA_H
#define HELLO_PIXYCAMERA_H

#include <opencv/cv.h>
#include "Render.h"


using namespace cv;

class PixyCamera {

    static const int width = 320;
    static const int height = 200;

public:
    PixyCamera()
            : m_render(Render()) {}

    int Test();
    int Stop();
    int Recording();
    Mat GetOneFrame();

private:
    Render m_render;
    int TestInit();
    int TestStop();
    int TestExposure();

};


#endif //HELLO_PIXYCAMERA_H
