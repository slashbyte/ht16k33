/*
 * ht16k33.h - used to talk to the htk1633 chip to do things like turn on LEDs or scan keys
 * Copyright:  Peter Sjoberg <peters-alib AT techwiz.ca>
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
 *
 * History:
 * 2015-10-04  Peter Sjoberg <peters-alib AT techwiz.ca>
 *             Created using https://www.arduino.cc/en/Hacking/LibraryTutorial and ht16k33 datasheet
 * 2015-11-25  Peter Sjoberg <peters-alib AT techwiz DOT ca>
 *	       first check in to github
 * 2016-08-09  René Wennekes <rene.wennekes AT gmail.com>
 *             Contribution of 7-segment & 16-segment display support
 *             Added clearAll() function
 *
 */
/*
Description: Pi adaptation of the arduino ht16k33 library
Author: Slash/Byte
Date: 11-9-2018
Notes: for use with the 4 character alphanumeric displays
*Requires wiringPi library!

 Tested display output, everything works for the 14seg display
 ...only the 14 seg display.... its hardware specific
 I have yet to test the key scanning, if I'm reading it right, it should work.
 but its not tested yet for the PI

.▄▄ · ▄▄▌   ▄▄▄· .▄▄ ·  ▄ .▄▄▄▄▄·  ▄· ▄▌▄▄▄▄▄▄▄▄ .
▐█ ▀. ██•  ▐█ ▀█ ▐█ ▀. ██▪▐█▐█ ▀█▪▐█▪██▌•██  ▀▄.▀·
▄▀▀▀█▄██▪  ▄█▀▀█ ▄▀▀▀█▄██▀▐█▐█▀▀█▄▐█▌▐█▪ ▐█.▪▐▀▀▪▄
▐█▄▪▐█▐█▌▐▌▐█ ▪▐▌▐█▄▪▐███▌▐▀██▄▪▐█ ▐█▀·. ▐█▌·▐█▄▄▌
 ▀▀▀▀ .▀▀▀  ▀  ▀  ▀▀▀▀ ▀▀▀ ··▀▀▀▀   ▀ •  ▀▀▀  ▀▀▀
*/


#ifndef ht16k33_h
#define ht16k33_h

#include <stdint.h>
#include <string>

class HT16K33
{
public:
    typedef uint16_t KEYDATA[3];

    HT16K33(); // the class itself

    //void    begin(uint8_t address, int fd);
    void begin(int fd, uint8_t chars); //send in the "standard Linux filehandle", and the total number of chars your display has, ie 4.
    void    end();
    void    clearAll(); // clear all LEDs
    uint8_t sleep();  // stop oscillator to put the chip to sleep
    uint8_t normal(); // wake up chip and start ocillator
    uint8_t clearLed(uint8_t ledno, uint8_t chars); //clears n-th led on n-th char
    uint8_t setLed(uint8_t ledno, uint8_t chars); //set n-th led on n-th char
    //boolean getLed(uint8_t ledno,boolean Fresh=false); // check if a specific led is on(true) or off(false)
    uint8_t sendLed(); // send whatever led patter you set
    uint8_t setLedNow(uint8_t ledno, uint8_t chars); //Set a single led and send led in one function, to n-th char
    uint8_t clearLedNow(uint8_t ledno, uint8_t chars); //Clear a single led and send led in one function, to n-th char
    uint8_t setBrightness(uint8_t level); // level 0-16, 0 means display off
    uint8_t keyINTflag(); // INTerrupt flag value, set when a key is pressed
    uint8_t keysPressed(); // report how many keys that are pressed, clear means report as if new
    int8_t  readKey(bool clear=false);  // read what key was pressed, Fresh=false to go from cache
    void    readKeyRaw(KEYDATA keydata,bool Fresh=true); //read the raw key info, bitmapped info of all key(s) pressed
    uint8_t setBlinkRate(uint8_t rate); // HT16K33_DSP_{NOBLINK,BLINK2HZ,BLINK1HZ,BLINK05HZ}
    void    displayOn();
    void    displayOff();
    // Some helper functions that can be useful in other parts of the code that use this library
    uint8_t i2c_write(uint8_t val);
    uint8_t i2c_write(uint8_t cmd, uint16_t *data, uint8_t size);
    uint8_t i2c_write(uint16_t data, uint8_t addr);
    uint8_t i2c_read(uint8_t addr);
    uint8_t i2c_read(uint8_t addr,uint8_t *data,uint8_t size);

    void testing(void);
    uint8_t alphaPrint(uint8_t chars, char value); //write char to buffer and display (14 seg font)
    uint8_t alphaSet(uint8_t chars, char value); //write char to memory
    uint8_t alphaPrintStr(std::string value); //writes string to the buffer and then displays it
    uint8_t alphaSetStr(std::string value); //writes string to memory
    void shiftRight(); //shift buffer to the right
    void shiftLeft(); //shift buffer to the left
    uint8_t moveRight(); //move display to the right
    uint8_t moveLeft(); //move display to the left
    uint8_t alphaPrintL(char value); //left justified printing, shift left
    uint8_t alphaPrintR(char value); //left justified printing, shift right
    uint8_t alphaPrintSL(std::string value, int _delay); //left scrolling string with delay
    uint8_t alphaPrintSR(std::string value, int _delay); //right scrolling string with delay

private:
    void _updateKeyram();
    KEYDATA _keyram;

    int _fd; //"standard Linux filehandle"
    uint16_t *displayRam_t;
    uint8_t _ramSize;
};


#endif
