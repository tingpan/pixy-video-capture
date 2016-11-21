//
// Created by TingMiao on 20/11/2016.
//
#include <string>
#include <iostream>
#include "PixyCamera.h"
#include "pixy.h"
#include <thread>
#include <sys/stat.h>
#include "colors.h"

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

    recording_flag = true;
    pixy_rcs_set_position(1, 900);
    pixy_rcs_set_position(0, 500);
    const String OUTPUT_FOLDER = "./output/";
    const String TIME = currentDateTime();
    const String suffix = ".avi";

    int fps = floor(getFPS());

    mkdir(OUTPUT_FOLDER.c_str(), 0777);
    mkdir((OUTPUT_FOLDER + TIME).c_str(), 0777);
    chdir((OUTPUT_FOLDER + TIME).c_str());

    VideoWriter outputVideo;                                        // Open the output
    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    const String NAME =  to_string(ms.count()) + suffix;

    Size S = Size(318, 198);

    outputVideo.open(NAME, CV_FOURCC('D','I','V','X'), fps, S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << endl;
        return -1;
    }

    cout << "Video: " << NAME <<", Input frame resolution: Width=" << S.width << "  Height=" << S.height << endl;
    std::thread t(&PixyCamera::writeVideo, this, outputVideo);

    cout << BOLD(FBLU("Press any q to stop recording")) << endl;

    while(getchar() != 'q');

    StopRecording();
    t.join();

    chdir("../../");
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
    int return_value;
    unsigned char *pixels;  //returned pointer to video frame buffer
    int32_t response, fourcc;
    int8_t renderflags;
    uint16_t width, height;
    uint32_t  numPixels;

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

//    printf("getFrame returned %d response %d\n", return_value, response);
//    printf("returned w %d h %d npix %d\n",width, height,numPixels);

    Mat raw_frame = m_render.renderBA81(renderflags, width, height, numPixels, pixels);
    m_render.renderTimeStamp(raw_frame);

    return raw_frame;
}

double PixyCamera::getFPS() {

    cout << FGRN("testing fps ...") << endl;

    long time_diff;
    double fps = 0;
    uint8_t test_frames = 50;
    milliseconds start = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

    for(long i = 0; i < test_frames; i++) //Show the image captured in the window and repeat
    {
        GetOneFrame();
    }

    milliseconds end = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

    time_diff = end.count() - start.count();
    fps = test_frames / (time_diff * 1.0 / 1000);

    printf("finish %d frames in %ld seconds, fps: %f \n", test_frames, time_diff, fps);

    return fps;
}

void PixyCamera::StopRecording() {
    recording_flag = false;
    cout << "stopping recording" << endl;
}

int PixyCamera::writeVideo(VideoWriter outputVideo) {

    for(long i = 0;; i++)
    {
        if (!recording_flag) break;

        if (i % 10 == 0) cout << i << " frames outputed" << recording_flag << endl;

        Mat pixy_image = GetOneFrame();

        outputVideo << pixy_image;
    }

    outputVideo.release();
    cout << "Finished writing" << endl;
    return 0;
}

int PixyCamera::Preview() {

    namedWindow("Recording", WINDOW_NORMAL);

    for(long i = 0;; i++)
    {
        if (i % 10 == 0) cout << i << " frames outputed" << recording_flag << endl;

        Mat pixy_image = GetOneFrame();

        imshow("Recording", pixy_image);

        if (waitKey(30) == 27 ) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;

            break;
        }
    }

    cout << "Finished Preview" << endl;
    return 0;


}

