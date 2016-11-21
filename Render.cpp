//
// Created by TingMiao on 21/11/2016.
//

#include <string>
#include <iostream>
#include <chrono>
#include <opencv2/imgproc.hpp>
#include "Render.h"

using namespace std;
using namespace std::chrono;

void Render::renderTimeStamp(Mat mat) {

    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    String s = to_string(ms.count());

    putText(mat, s, cvPoint(5,190),
            FONT_HERSHEY_SIMPLEX, 0.3, cvScalar(255,255,255), 1, CV_AA);

}

Mat Render::renderBA81(int8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame) {

    uint16_t x, y;
    uint8_t r, g, b;
    Mat imageRGB;

    frame += width;

    uchar data[3 * ((height-2) * (width-2)) ];

    uint m = 0;
    for (y = 1; y < height - 1; y++)
    {
        frame++;
        for (x = 1; x < width - 1; x++, frame++)
        {
            interpolateBayer(width, x, y, frame, &r, &g, &b);
            data[m++] = b;
            data[m++] = g;
            data[m++] = r;

        }
        frame++;
    }

    imageRGB = Mat(height - 2,width -2, CV_8UC3, data);
    return imageRGB;
}

void Render::interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t *r, uint8_t *g,
                                  uint8_t *b) {
    if (y&1)
    {
        if (x&1)
        {
            *r = *pixel;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
        }
        else
        {
            *r = (*(pixel-1)+*(pixel+1))>>1;
            *g = *pixel;
            *b = (*(pixel-width)+*(pixel+width))>>1;
        }
    }
    else
    {
        if (x&1)
        {
            *r = (*(pixel-width)+*(pixel+width))>>1;
            *g = *pixel;
            *b = (*(pixel-1)+*(pixel+1))>>1;
        }
        else
        {
            *r = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = *pixel;
        }
    }

}