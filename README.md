
# Pixy Video Capture
This is a video capture script for pixy camera. Support preview and save video stream from PIXY.


##How to Config

### Dependencies (Debian)

#### Update apt-get
```
$ apt-get update
$ apt-get upgrade
```

#### Install Dependencies of OpenCV
```
$ apt-get install build-essential cmake pkg-config
$ apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
$ apt-get install libavcodec-dev libavformat-dev libswscale-dev
$ apt-get install libv4l-dev libxvidcore-dev libx264-dev libgtk2.0-dev libatlas-base-dev gfortran
$ apt-get install python2.7-dev python3-dev
```

#### Install and Compile OpenCV
```
$ git clone https://github.com/opencv/opencv.git
$ cd opencv
$ cmake .
$ make
$ make install
```

#### Install Dependencies of Pixy
```
$ apt-get install libusb-1.0-0-dev
$ apt-get install libboost-all-dev
$ git clone https://github.com/charmedlabs/pixy.git
$ cd pixy/scripts
$ ./build_libpixyusb
```

### Run the Program

Connect the Pixy to your device, then run

```
$ git clone https://github.com/tingpan/pixy_video_capture.git
$ cd pixy_video_capture
$ cmake .
$ make
$ sudo ./demo
```
That will give you a menu to operate, and you can:

1. type 'r' than 'Enter' to start record, you will see how many frames are outputed to the folder. type 'q' than 'Enter' to stop the record.
2. type 'p' than 'Enter' to preview the record, type 'Esc' to stop preview.
3. type 'e' than 'Enter' to exit the program.

