//
// Created by TingMiao on 20/11/2016.
//

#ifndef HELLO_PIXYCAMERA_H
#define HELLO_PIXYCAMERA_H

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "Render.h"

using namespace cv;

class PixyCamera {

    static const int width = 320;
    static const int height = 200;

public:
    PixyCamera()
            : m_render(Render()), recording_flag(false) {}

    int Test();
    int Stop();
    int Recording();
    int Preview();
    void StopRecording();
    Mat GetOneFrame();

private:
    Render m_render;
    bool recording_flag;

    int TestInit();
    int TestStop();
    int TestExposure();
    double getFPS();
    int writeVideo(VideoWriter outputVideo);

    const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
        return buf;
    }

};


#endif //HELLO_PIXYCAMERA_H
