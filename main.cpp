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
#include "colors.h"

using namespace std;
void showMenu();

int main() {

    PixyCamera p;

    if (p.Test() == 0) {

        int input;
        showMenu();

        while ((input = getchar()) != 'e') {

            switch (input) {
                case 'p':
                    p.Preview();
                    break;
                case 'r':
                    p.Recording();
                    break;
                default:
                    cout << FRED("invalid input") << endl;
                    showMenu();

            }
        }

        cout << "Exit" << endl;
    } else {
        cout << "Pixy camera error, exit";
        exit(-1);
    }
}

void showMenu() {
    cout << "p : preview the video" << endl;
    cout << "r : start recording" << endl;
    cout << "e : exit the program" << endl;
}