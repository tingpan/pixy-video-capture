//
// Created by TingMiao on 21/11/2016.
//

#ifndef HELLO_RENDER_H
#define HELLO_RENDER_H

#include <opencv/cv.h>

using namespace cv;

#include <cstdint>

class Render {

public:
    void renderTimeStamp(Mat mat);
    Mat renderBA81(int8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);
    inline void interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t* r, uint8_t* g, uint8_t* b);
};


#endif //HELLO_RENDER_H
