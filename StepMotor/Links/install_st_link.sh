#!/bin/bash

sudo apt-get install git make cmake libusb-1.0-0-dev
sudo apt-get install gcc build-essential

cd tmp
mkdir stm32
cd stm32
git clone https://github.com/stlink-org/stlink
cd stlink
cmake .
make

cd bin
sudo cp st-* /usr/local/bin
cd ../lib
sudo cp *.so* /lib32

sudo cp ../config/udev/rules.d/49-stlinkv* /etc/udev/rules.d/

lsusb