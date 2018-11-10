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
This demo scrolls text across a 4 char 14 segment display
I call it the "super happy fun time demo!"
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
    //display string
    string str = "SUPER HAPPY fun TIME demo    ";
    //scroll loop
    printf("string: %s\n", str.c_str());
    int loopCnt = 5;
    for(int i = 0; i < loopCnt; i++)
    {
        printf("loop count: %d\n", loopCnt-i); //loop counter
        HT.alphaPrintSL(str, 150);
    }
    return 0;
}