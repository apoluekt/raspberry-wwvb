# raspberry-wwvb
WWVB clock signal transmitter on Raspberry Pi

## Introduction



## Prerequisites

The code uses WiringPi library to control hardware PWM of Raspberry Pi: http://wiringpi.com/download-and-install/

## Hardware setup

Plug a wire to the Raspberry Pi GPIO pin 18, it will serve as an antenna. 

## Software setup

Run ./build.sh script to compile the WWVB program. You will need to run it as root: 

> sudo /home/pi/wwvb
