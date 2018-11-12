/*
 * main.cpp - used to talk to the htk1633 chip to do things like turn on LEDs or scan keys
 * Copyright:  Slash/Byte
 * License: GPLv3
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 	.▄▄ · ▄▄▌   ▄▄▄· .▄▄ ·  ▄ .▄▄▄▄▄·  ▄· ▄▌▄▄▄▄▄▄▄▄ .
 *  ▐█ ▀. ██•  ▐█ ▀█ ▐█ ▀. ██▪▐█▐█ ▀█▪▐█▪██▌•██  ▀▄.▀·
 *  ▄▀▀▀█▄██▪  ▄█▀▀█ ▄▀▀▀█▄██▀▐█▐█▀▀█▄▐█▌▐█▪ ▐█.▪▐▀▀▪▄
 *  ▐█▄▪▐█▐█▌▐▌▐█ ▪▐▌▐█▄▪▐███▌▐▀██▄▪▐█ ▐█▀·. ▐█▌·▐█▄▄▌
 *   ▀▀▀▀ .▀▀▀  ▀  ▀  ▀▀▀▀ ▀▀▀ ··▀▀▀▀   ▀ •  ▀▀▀  ▀▀▀
 *                                         11-11-2018
 *     Super happy fun time test demo extravaganza
 * 				       For The
 * "Adafruit 0.54" Quad Alphanumeric FeatherWing Display - Red"
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string>
#include "ht16k33.h"
#include "starburst.h"

#define ADDR 0x70

int main()
{
    STARBURST HT;
    HT.begin(ADDR, 4); //address of the display and the number of digits
    //dialog
    printf("2018, Slash/Byte\n");
    printf("Welcome to the test program...\n");
    HT.delay(500);
    //clear all test
    printf("Clear all test.\n");
    HT.clrAll();
    //string test
    printf("String test.\n");
    HT.print("/bit");
    HT.delay(500);
    HT.print("demo");
    HT.delay(500);
    //string scroll test
    printf("Scroll test.\n");
    HT.print("ALL WORK AND NO PLAY, MAKES JACK A DULL BOY", 150);
    //DP test
    printf("DeadPool test.\n");
    HT.print('3', 0, 1);
    HT.print('1', 1);
    HT.print('4', 2);
    HT.print('1', 3);
    HT.delay(500);
    HT.clrAll();
    HT.delay(100);
    //segment test
    printf("Indexed LED test.\n");
    for(int i = 0; i < 64; i++) //16(bits)*4(digits)
    {
        HT.setLed(i);
        HT.delay(20);
    }
    //blink test
    printf("Blink test.\n");
    HT.setBlinkRate(3); //2hz blink
    HT.delay(2000);
    HT.setBlinkRate(0); //0hz blink
    //dump test
    printf("Dump test.\n");
    HT.keyDump();
    HT.memDump();
    //random bits
    printf("And now for something completely different.\n");
    HT.clrAll();
    //sudo random data stream from Linear-feedback shift register
    uint16_t lfsr = 0xACE1u;
    for(int i = 0; i < 125; i++) //lasts 15 seconds
    {
        unsigned lsb = lfsr & 1;
        lfsr >>= 1;
        if(lsb)
            lfsr ^=0xB400u;
        HT.shiftMR();
        HT.HT16K33::memory[0] = lfsr & 0x3FFF;
        HT.update();
        HT.delay(120);
    }
    HT.memDump();

    return 0;
}
