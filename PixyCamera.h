//
// Created by TingMiao on 20/11/2016.
//

#ifndef HELLO_PIXYCAMERA_H
#define HELLO_PIXYCAMERA_H

#include <opencv/cv.h>


using namespace cv;

class PixyCamera {

public:
    int Test();
    int Stop();
    int Recording();
    Mat GetOneFrame();

private:
    int TestInit();
    int TestStop();
    int TestExposure();
    Mat renderBA81(int8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);
    inline void interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t* r, uint8_t* g, uint8_t* b);

};


#endif //HELLO_PIXYCAMERA_H
