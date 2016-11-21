//
// Created by TingMiao on 20/11/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "PixyCamera.h"

using namespace std;



int main() {

    PixyCamera p;

    if (p.Test() == 0) {

        p.Recording();
        waitKey(0);

    } else {
        cout << "Pixy camera error, exit";
        exit(-1);
    }
}