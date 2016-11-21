//
// Created by TingMiao on 20/11/2016.
//
#include <string>
#include <iostream>
#include "PixyCamera.h"
#include <math.h>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "pixy.h"

using namespace cv;
using namespace std;

int PixyCamera::Test() {

    int return_value = 0;
    int t = 0;

    cout << "testing Pixy...\n";

    // Connect to Pixy //
    while (t < 3 && return_value == 0) {

        switch(t) {
            case 0:
                return_value = TestInit();
                break;
            case 1:
                return_value = TestStop();
                break;
            case 2:
                return_value = TestExposure();
                break;
            default:
                break;
        }

        t++;
    }

    if (return_value == 0) {
        cout << "tesing Pixy success\n";

    } else {
        cout << "tesing Pixy failed\n";
    }

    return return_value;
}

int PixyCamera::Stop() {

    int return_value;
    int32_t response;

    return_value = pixy_command("stop", END_OUT_ARGS, &response, END_IN_ARGS);

    printf("STOP returned %d response %d\n", return_value, response);

    return return_value;
}

int PixyCamera::Recording() {

    pixy_rcs_set_position(1, 900);
    pixy_rcs_set_position(0, 500);

    Mat pixy_image = GetOneFrame();

    namedWindow("Image", CV_WINDOW_NORMAL);
    imshow("Image", pixy_image);

    cout << "ss";
    return 0;

}

int PixyCamera::TestInit() {

    int return_value;

    return_value = pixy_init();

    printf("initialized Pixy result : %d\n", return_value);

    if(return_value != 0)
    {
        // Error initializing Pixy //
        cout << "pixy init error: ";
        pixy_error(return_value);
    }

    return return_value;
}

int PixyCamera::TestStop() {

    int return_value = this->Stop();

    if(return_value != 0)
    {
        // Error initializing Pixy //
        cout << "pixy stop error: ";
        pixy_error(return_value);
    }

    return return_value;
}

int PixyCamera::TestExposure() {

    int return_value;
    uint8_t gain;
    uint16_t compensation;

    return_value = pixy_cam_get_exposure_compensation (&gain, &compensation);
    printf("getECV returned %d values: 0, %d, %d\n", return_value, gain, compensation);

    if(return_value != 0)
    {
        // Error initializing Pixy //
        cout << "pixy exposure error: ";
        pixy_error(return_value);
    }

    return return_value;
}

Mat PixyCamera::GetOneFrame() {

    unsigned char current_frame[72000]; // ~largest possible given current hardware
    unsigned char *pixels;  //returned pointer to video frame buffer
    int32_t response, fourcc;
    int8_t renderflags;
    int return_value, res;
    uint16_t width, height;
    uint32_t  numPixels;

//  stop blob processing
    return_value = pixy_command("stop", END_OUT_ARGS, &response, END_IN_ARGS);
    printf("STOP returned %d response %d\n", return_value, response);

    response = 0;
    return_value = pixy_command("cam_getFrame",  // String id for remote procedure
                                0x01, 0x21,      // mode 0 = 1280x800 25 fps
                                0x02,   0,        // xoffset
                                0x02,   0,         // yoffset
                                0x02, 320,       // width
                                0x02, 200,       // height (56 max @ 1280 w)
                                0,              // separator
                                &response,      // pointer to mem address for return value
                                &fourcc,         //contrary to docs, the next 5 args are needed
                                &renderflags,
                                &width,
                                &height,
                                &numPixels,
                                &pixels,        // pointer to mem address for returned frame
                                0);

    printf("getFrame returned %d response %d\n", return_value, response);
    printf("returned w %d h %d npix %d\n",width, height,numPixels);

    // quit now if not successful:
    if(return_value != 0)
    {
        // Error initializing Pixy //
        cout << "pixy exposure error: ";
        pixy_error(return_value);
    }

    return renderBA81(renderflags, width, height, numPixels, pixels);
}


Mat PixyCamera::renderBA81(int8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame) {

    uint16_t x, y;
    uint8_t r, g, b;
    Mat imageRGB;

    frame += width;

    uchar data[3*((height-2)*(width-2))];

    uint m = 0;
    for (y = 1; y < height-1; y++)
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

void PixyCamera::interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t *r, uint8_t *g,
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
