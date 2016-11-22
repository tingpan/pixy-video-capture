
##How to Config

#### Network
sudo nano /etc/apt-apt.conf 
comment with # any line which contains wwwcache.cs.nott.ac.uk
save and exit

#### Packages
sudo apt-get update
sudo apt-get upgrade

sudo apt-get install build-essential cmake pkg-config
sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev 
sudo apt-get install libv4l-dev libxvidcore-dev libx264-dev libgtk2.0-dev libatlas-base-dev gfortran
sudo apt-get install python2.7-dev python3-dev

git clone https://github.com/opencv/opencv.git
cd opencv
cmake .
make
make install

libusb : sudo apt-get install libusb-1.0-0-dev

libboost : sudo apt-get install libboost-all-dev

git clone https://github.com/charmedlabs/pixy.git
cd pixy/scripts
./build_libpixyusb

correct the device time : npdate 0.uk.pool.ntp.org
