//
// Created by TingMiao on 20/11/2016.
//
#include <string>
#include <iostream>
#include "PixyCamera.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "pixy.h"

using namespace cv;
using namespace std;
using namespace std::chrono;

int PixyCamera::Test() {

    int return_value = 0;
    int t = 0;

    cout << "testing Pixy...\n";

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
    namedWindow("Image", WINDOW_NORMAL);

    VideoWriter outputVideo;                                        // Open the output
    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    const String NAME =  "./output/" + to_string(ms.count()) + ".avi";
    Size S = Size(318, 198);

    outputVideo.open(NAME, CV_FOURCC('D','I','V','X'), 20, S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: ";
        return -1;
    }

    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height;

    for(;;) //Show the image captured in the window and repeat
    {

        Mat pixy_image = GetOneFrame();

        //outputVideo.write(res); //save or

        outputVideo << pixy_image;

        imshow("Image", pixy_image);

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;

            break;
        }
    }
    cout << "Finished writing" << endl;
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

    Mat raw_frame = m_render.renderBA81(renderflags, width, height, numPixels, pixels);
    m_render.renderTimeStamp(raw_frame);

    return raw_frame;

}

