# raspberry-wwvb
WWVB clock signal transmitter on Raspberry Pi

## Introduction

WWVB is a US station that transmits radio signals that can be received by radio-controlled clocks to adjust their time: https://en.wikipedia.org/wiki/WWVB. This project uses Raspberry Pi to mimic the WWVB signal: it could be useful in cases when real WWVB signal is too weak or cannot be received. For instance, I use Raspberry Pi to set correct time of my Casio WaveCeptor watch desined for US market (that can only receive WWVB signals) outside of US. 

## Prerequisites

The code uses WiringPi library to control hardware PWM of Raspberry Pi. WiringPi should be installed prior to compiling the code. Installation instructions are here: http://wiringpi.com/download-and-install/

## Hardware setup

Plug a wire to the Raspberry Pi GPIO pin 18 (PWM0), it will serve as an antenna. See here to locate the pin: 
https://pinout.xyz/pinout/pin12_gpio18. Since the transmitted signal is very weak, you need to place your clock/watch maximum a few centimeters away from the antenna wire. 

## Software setup

Run ./build.sh script to compile the WWVB program. Once the program is built, you will need to run it as root: 

> sudo /home/pi/wwvb

In its current configuration, the program will transmit the signal for 10 minutes, after which it will exit. Normally 10 minutes is sufficient to adjust the clock. 

It is convenient to run the program around the time when the signal is expected to be received by the clock. My Casio WaveSeptor watch try to receive the signal for a few minutes at the start of every hour during the night (12am, 1am, ... 5am), so I set up cron to run the transmitter around these times. Here's the extract of my crontab: 

    # m h  dom mon dow   command

    59 23 * * * sudo /home/pi/wwvb
    59 00 * * * sudo /home/pi/wwvb
    59 01 * * * sudo /home/pi/wwvb
    59 02 * * * sudo /home/pi/wwvb
    59 03 * * * sudo /home/pi/wwvb
    59 04 * * * sudo /home/pi/wwvb

