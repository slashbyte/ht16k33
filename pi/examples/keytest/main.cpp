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
This is a key test, I'm not sure if it works or not, I can't test it
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
	printf("Well go-ahead! push some buttons!\n");
	printf("[Ctrl]+C to exit the loop of doom\n");
    HT.alphaPrintStr("Keys"); //display something stupid

    int8_t key;
    uint8_t lastKey = HT.readKey();

    while(1) //run loop
    {
        key = HT.readKey();
        if(key != 0)
        {
            if(key != lastKey)
            {
                printf("Key pressed: %d\n", key);
                lastKey = key;
            }
        }
    }
    return 0;
}