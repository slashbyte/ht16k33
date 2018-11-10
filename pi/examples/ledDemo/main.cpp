/*
File: main.cpp
Description: Pi adaptation of the arduino ht16k33 library
Author: Slash/Byte
Date: 11-9-2018
Notes: for use with the 4 character alphanumeric displays
*Requires wiringPi library!

Hardware: "Quad Alphanumeric FeatherWing Display"
https://www.adafruit.com/product/3130

chip states 5v operation, adafruit claims it runs on 3.3v.
Works on the PI!!!!!!

.▄▄ · ▄▄▌   ▄▄▄· .▄▄ ·  ▄ .▄▄▄▄▄·  ▄· ▄▌▄▄▄▄▄▄▄▄ .
▐█ ▀. ██•  ▐█ ▀█ ▐█ ▀. ██▪▐█▐█ ▀█▪▐█▪██▌•██  ▀▄.▀·
▄▀▀▀█▄██▪  ▄█▀▀█ ▄▀▀▀█▄██▀▐█▐█▀▀█▄▐█▌▐█▪ ▐█.▪▐▀▀▪▄
▐█▄▪▐█▐█▌▐▌▐█ ▪▐▌▐█▄▪▐███▌▐▀██▄▪▐█ ▐█▀·. ▐█▌·▐█▄▄▌
 ▀▀▀▀ .▀▀▀  ▀  ▀  ▀▀▀▀ ▀▀▀ ··▀▀▀▀   ▀ •  ▀▀▀  ▀▀▀
*/

/*
This is a segment/led demo
*/

#include "ht16k33.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>

using namespace std;

#define ADDR 0x70 //address of i2c hardware

int main()
{
    int fd;
    wiringPiSetup();
    fd = wiringPiI2CSetup(ADDR);
    // Define the class
    HT16K33 HT;
    HT.begin(fd, 4); //using 4 char display
    printf("Slash/Byte\n");
    printf("now cycling through all the LEDS\n");
    int delayTime = 100;

    //turns all on
    printf("All ON!\n");
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            HT.setLedNow(j,i); //j=segment, i=character
            delay(delayTime);
        }
    }
    delay(delayTime);

    printf("Brightness Test!\n");
    for(int i = 0; i < 16; i++)
    {
        printf("brightness: %d\n", i);
        HT.setBrightness(i);
        delay(delayTime*3);
    }
    HT.setBrightness(15); //max

    printf("Blink!\n");
    HT.setBlinkRate(0x02);
    delay(2000); //2sec
    HT.setBlinkRate(0x00);

    //turns all off
    printf("All OFF!\n");
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            HT.clearLedNow(j,i); //j=segment, i=character
            delay(delayTime);
        }
    }
    delay(delayTime);

    return 0;
}