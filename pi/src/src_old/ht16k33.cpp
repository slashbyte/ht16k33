/*
 * ht16k33.cpp - used to talk to the htk1633 chip to do things like turn on LEDs or scan keys
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

#include "ht16k33.h"
#include "font.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <string>

#define SETBIT(x,n) x |= (1 << n)
#define CLRBIT(x,n) x &= ~(1 << n)

// "address" is base address 0-7 which becomes 11100xxx = E0-E7
//#define BASEHTADDR 0x70

//Commands
#define HT16K33_DDAP          0x00 // B00000000 // Display data address pointer: 0000xxxx
#define HT16K33_SS            0x20 // B00100000 // System setup register
#define HT16K33_SS_STANDBY    0x00 // B00000000 // System setup - oscillator in standby mode
#define HT16K33_SS_NORMAL     0x01 // B00000001 // System setup - oscillator in normal mode
#define HT16K33_KDAP          0x40 // B01000000 // Key Address Data Pointer
#define HT16K33_IFAP          0x60 // B01100000 // Read status of INT flag
#define HT16K33_DSP           0x80 // B10000000 // Display setup
#define HT16K33_DSP_OFF       0x00 // B00000000 // Display setup - display off
#define HT16K33_DSP_ON        0x01 // B00000001 // Display setup - display on
#define HT16K33_DSP_NOBLINK   0x00 // B00000000 // Display setup - no blink
#define HT16K33_DSP_BLINK2HZ  0x02 // B00000010 // Display setup - 2hz blink
#define HT16K33_DSP_BLINK1HZ  0x04 // B00000100 // Display setup - 1hz blink
#define HT16K33_DSP_BLINK05HZ 0x06 // B00000110 // Display setup - 0.5hz blink
#define HT16K33_RIS           0xA0 // B10100000 // ROW/INT Set
#define HT16K33_RIS_OUT       0x00 // B00000000 // Set INT as row driver output
#define HT16K33_RIS_INTL      0x01 // B00000001 // Set INT as int active low
#define HT16K33_RIS_INTH      0x03 // B00000011 // Set INT as int active high
#define HT16K33_DIM           0xE0 // B11100000 // Dimming set
#define HT16K33_DIM_1         0x00 // B00000000 // Dimming set - 1/16
#define HT16K33_DIM_2         0x01 // B00000001 // Dimming set - 2/16
#define HT16K33_DIM_3         0x02 // B00000010 // Dimming set - 3/16
#define HT16K33_DIM_4         0x03 // B00000011 // Dimming set - 4/16
#define HT16K33_DIM_5         0x04 // B00000100 // Dimming set - 5/16
#define HT16K33_DIM_6         0x05 // B00000101 // Dimming set - 6/16
#define HT16K33_DIM_7         0x06 // B00000110 // Dimming set - 7/16
#define HT16K33_DIM_8         0x07 // B00000111 // Dimming set - 8/16
#define HT16K33_DIM_9         0x08 // B00001000 // Dimming set - 9/16
#define HT16K33_DIM_10        0x09 // B00001001 // Dimming set - 10/16
#define HT16K33_DIM_11        0x0A // B00001010 // Dimming set - 11/16
#define HT16K33_DIM_12        0x0B // B00001011 // Dimming set - 12/16
#define HT16K33_DIM_13        0x0C // B00001100 // Dimming set - 13/16
#define HT16K33_DIM_14        0x0D // B00001101 // Dimming set - 14/16
#define HT16K33_DIM_15        0x0E // B00001110 // Dimming set - 15/16
#define HT16K33_DIM_16        0x0F // B00001111 // Dimming set - 16/16

// Constructor
HT16K33::HT16K33()
{
    //somethings here now!!!!!
}

/****************************************************************/
// Setup the env
//
void HT16K33::begin(int fd, uint8_t chars)
{
    _ramSize = chars;
    displayRam_t = new uint16_t[_ramSize];
    _fd = fd;
    i2c_write(HT16K33_SS  | HT16K33_SS_NORMAL); // Wakeup
    i2c_write(HT16K33_DSP | HT16K33_DSP_ON | HT16K33_DSP_NOBLINK); // Display on and no blinking
    i2c_write(HT16K33_RIS | HT16K33_RIS_OUT); // INT pin works as row output
    i2c_write(HT16K33_DIM | HT16K33_DIM_16);  // Brightness set to max
    clearAll();
} // begin

/****************************************************************/
// internal function - Write a single byte
//
uint8_t HT16K33::i2c_write(uint8_t val)
{
    return wiringPiI2CWrite(_fd, val);
} // i2c_write

/****************************************************************/
// internal function - Write several bytes
// "size" is amount of data to send excluding the first command byte
//
uint8_t HT16K33::i2c_write(uint8_t cmd, uint16_t *data, uint8_t size)
{
    uint8_t i = 0;
    i|= wiringPiI2CWrite(_fd, cmd);
    for(int j = 0; j < size; j++)
        i|= wiringPiI2CWriteReg16(_fd, (j*2),data[j]); // Send out the data
    return i;
} // i2c_write

/****************************************************************/
// internal function - Write 2bytes to address
//
uint8_t HT16K33::i2c_write(uint16_t data, uint8_t addr)
{
    return wiringPiI2CWriteReg16(_fd, addr, data);
} // i2c_write

/****************************************************************/
// internal function - read a byte from specific address (send one byte(address to read) and read a byte)
//
uint8_t HT16K33::i2c_read(uint8_t addr)
{
    return wiringPiI2CReadReg8(_fd, addr);
} // i2c_read

/****************************************************************/
// read an array from specific address (send a byte and read a bytes back)
// return value is how many bytes that where really read... I guess?
// ...not as pretty as yours!
//
uint8_t HT16K33::i2c_read(uint8_t addr, uint8_t *data, uint8_t size)
{
    for(uint8_t i = 0; i < size; i++)
    {
        data[i] = i2c_read((addr+i));
    }
    return size;
} // i2c_read

/****************************************************************/
// Clear all leds and displays
//
void HT16K33::clearAll()
{
    for(int i=0; i<_ramSize; i++)
        displayRam_t[i] = 0x0000; //memset only dose 8bits
    i2c_write(HT16K33_DDAP, displayRam_t, _ramSize); //elements in array
} // clearAll

/****************************************************************/
// Put the chip to sleep
//
uint8_t HT16K33::sleep()
{
    return i2c_write(HT16K33_SS|HT16K33_SS_STANDBY); // Stop oscillator
} // sleep

/****************************************************************/
// Wake up the chip (after it been a sleep )
//
uint8_t HT16K33::normal()
{
    return i2c_write(HT16K33_SS|HT16K33_SS_NORMAL); // Start oscillator
} // normal

/****************************************************************/
// Turn off one led but only in memory
// To do it on chip a call to "sendLed" is needed
//
uint8_t HT16K33::clearLed(uint8_t ledno, uint8_t chars)  // 16x8 = 128 LEDs to turn on, 0-127
{
    if((ledno >=0) && (ledno < 15)) //14 se + dp
        if((chars >=0) && (chars < _ramSize))
        {
            CLRBIT(displayRam_t[chars], ledno);
            return 0;
        }
    return 1;
} // clearLed

/****************************************************************/
// Turn on one led but only in memory
// To do it on chip a call to "sendLed" is needed
//
uint8_t HT16K33::setLed(uint8_t ledno, uint8_t chars)  //
{
    if((ledno >=0) && (ledno < 15)) //14 se + dp
        if((chars >=0) && (chars < _ramSize))
        {
            SETBIT(displayRam_t[chars], ledno);
            return 0;
        }
    return 1;
} // setLed

/****************************************************************/
// check if a specific led is on(true) or off(false)
//
//oh shit! I get it, its like a mem dump 'n' compare
//to bad I kinda broke how the library works, might fix it later.
/*
boolean HT16K33::getLed(uint8_t ledno,boolean Fresh){

  // get the current state from chip
  if (Fresh) {
    i2c_read(HT16K33_DDAP, displayRam,sizeof(displayRam));
  }

  if (ledno>=0 && ledno<128){
    return bitRead(displayRam[int(ledno/8)],ledno % 8) != 0;
  }
} // getLed
*/
/****************************************************************/
// Send the display ram info to chip - kind of commit all changes to the outside world
//
uint8_t HT16K33::sendLed()
{
    return i2c_write(HT16K33_DDAP, displayRam_t ,_ramSize);
} // sendLed

/****************************************************************/
// set a single LED and update NOW
//
uint8_t HT16K33::setLedNow(uint8_t ledno, uint8_t chars)
{
    uint8_t rc;
    rc=setLed(ledno, chars);
    if (rc==0)
        return sendLed();
    return rc;
} // setLedNow

/****************************************************************/
// clear a single LED and update NOW
//
uint8_t HT16K33::clearLedNow(uint8_t ledno, uint8_t chars)
{
    uint8_t rc;
    rc=clearLed(ledno, chars);
    if (rc==0)
        return sendLed();
    return rc;
} // clearLedNow

/****************************************************************/
// Change brightness of the whole display
// level 0-15, 0 means display off
//
uint8_t HT16K33::setBrightness(uint8_t level)
{
    if (HT16K33_DIM_1>=0 && level <HT16K33_DIM_16)
        return i2c_write(HT16K33_DIM|level);
    return 1;
} // setBrightness

/****************************************************************/
// Check the chips interrupt flag
// 0 if no new key is pressed
// !0 if some key is pressed and not yet read
//
uint8_t HT16K33::keyINTflag()
{
    return i2c_read(HT16K33_IFAP);
} // keyINTflag

/****************************************************************/
// Check if any key is pressed
// returns how many keys that are currently pressed
//

//From http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer

#ifdef __GNUC__
uint16_t _popcount(uint16_t x)   // funky functions!
{
    return __builtin_popcount(x);
}
#else
uint16_t _popcount(uint16_t i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
#endif

uint8_t HT16K33::keysPressed()
{
    //  printf("debug: 0x%.4X\n", _keyram[0]|_keyram[1]|_keyram[2]);
    return (_popcount(_keyram[0])+_popcount(_keyram[1])+_popcount(_keyram[2]));
} // keysPressed

/****************************************************************/
// Internal function - update cached key array
// I hope!
void HT16K33::_updateKeyram()
{
    uint8_t curkeyram[6];

    i2c_read(HT16K33_KDAP, curkeyram, 6);
    _keyram[0]=curkeyram[1]<<8 | curkeyram[0]; // datasheet page 21, 41H is high 40H is low
    _keyram[1]=curkeyram[3]<<8 | curkeyram[2]; // or LSB MSB
    _keyram[2]=curkeyram[5]<<8 | curkeyram[4];
    return;
} // _updateKeyram

/****************************************************************/
// return the key status
//
void HT16K33::readKeyRaw(HT16K33::KEYDATA keydata,bool Fresh)
{
    int8_t i;

    // get the current state
    if (Fresh)
        _updateKeyram();

    for (i=0; i<3; i++)
        keydata[i]=_keyram[i];

    return;
} // readKeyRaw

/****************************************************************
 * read the keys and return the key that changed state
 * if more than one is pressed (compared to last scan)
 * only one is returned, the first one found
 * 0 means no key pressed.
 * "1" means the key #1 is pressed
 * "-1" means the key #1 is released
 * "clear"=true means it will only look keys currently pressed down.
 *     this is so you can detect what key is still pressed down after
 *     several keys are pressed down and then all but one is released
 *     (without keeping track of up/down separately)
 *
 *Observations:
 * As long as the key is pressed the keyram bit is set
 * the flag is set when key is pressed down but then cleared at first
 * read of key ram.
 * When released the key corresponding bit is cleared but the flag is NOT set
 * This means that the only way a key release can be detected is
 * by only polling readKey and ignoring flag
 *
 */

int8_t HT16K33::readKey(bool clear)
{
    static HT16K33::KEYDATA oldKeyData;
    uint16_t diff;
    uint8_t key;
    int8_t i,j;

    // save the current state
    for (i=0; i<3; i++)
    {
        if (clear)
            oldKeyData[i]=0;
        else
            oldKeyData[i]=_keyram[i];
    }

    _updateKeyram();

    key=0; //the key that changed state
    for (i=0; i<3; i++)
    {
        diff=_keyram[i] ^ oldKeyData[i]; //XOR old and new, any changed bit is set.
        if ( diff !=0 )  // something did change
        {
            for (j=0; j<13; j++)
            {
                key++;
                if (((diff >> j) & 1) == 1)
                {
                    if (((_keyram[i] >> j) & 1)==0)
                    {
                        return -key;
                    }
                    else
                    {
                        return key;
                    }
                } // if keyram differs
            } // for j in bits
        }
        else
        {
            key+=13;
        } // if diff
    } // for i
    return 0; //apperently no new key was pressed - old might still be held down, pass clear=true to see it
} // readKey

/****************************************************************/
// Make the display blink
//
uint8_t HT16K33::setBlinkRate(uint8_t rate)
{
    switch (rate)
    {
    case HT16K33_DSP_NOBLINK:
    case HT16K33_DSP_BLINK2HZ:
    case HT16K33_DSP_BLINK1HZ:
    case HT16K33_DSP_BLINK05HZ:
        i2c_write(HT16K33_DSP | rate | HT16K33_DSP_ON);
        return 0;
        ;;
    default:
        return 1;
    }
} //setBlinkRate

/****************************************************************/
// turn on the display
//
void HT16K33::displayOn()
{
    i2c_write(HT16K33_DSP |HT16K33_DSP_ON);
} // displayOn

/****************************************************************/
// turn off the display
//
void HT16K33::displayOff()
{
    i2c_write(HT16K33_DSP |HT16K33_DSP_OFF);
} // displayOff
/****************************************************************/
// write a char to the buffer and to the display
// assuming 14 segment display, + dp
//
uint8_t HT16K33::alphaPrint(uint8_t chars, char value)
{
    uint8_t i = alphaSet(chars, value); //set ram
    if(!i)
        return i2c_write(displayRam_t[chars], chars*2); //send
    return 1;
}
/****************************************************************/
// write a char to the buffer, call sendLed() to update
// assuming 14 segment display, + dp
//
uint8_t HT16K33::alphaSet(uint8_t chars, char value)
{
    if((chars < 8) && (chars >= 0))
        if((value < 128) && (value >= 0))
        {
            displayRam_t[chars] = alphafonttable[(uint8_t)value]; //set ram
            return 0;
        }
    return 1;
}
/****************************************************************/
// writes a string to mem, then displays
//
uint8_t HT16K33::alphaPrintStr(std::string value)
{
    uint8_t i = alphaSetStr(value);
    i |= sendLed();
    return i;
}
/****************************************************************/
// writes a string to mem, if the string is to long, it gets clipped
// "sendLed" to update the display
//
uint8_t HT16K33::alphaSetStr(std::string value)
{
    uint8_t j=0;
    if(value.empty())
        return 1;
    if(value.length() > _ramSize)
        value.resize(_ramSize);
    for(int i = 0; i < _ramSize; i++)
        displayRam_t[i] = 0x0000; //clr memory
    for(int i = 0; i < (int)value.length(); i++)
        j|= alphaSet(i, value[i]);
    return j;
}
/****************************************************************/
//shift display buffer to the right
// To do it on chip a call to "sendLed" is needed
//
void HT16K33::shiftRight(void)
{
    for(int i = _ramSize; i > 0; i--)
        displayRam_t[i] = displayRam_t[i-1];
    displayRam_t[0] = 0x0000;
}
/****************************************************************/
//shift display buffer to the left
// To do it on chip a call to "sendLed" is needed
//
void HT16K33::shiftLeft(void)
{
    for(int i = 0; i < _ramSize-1; i++)
        displayRam_t[i] = displayRam_t[i+1];
    displayRam_t[_ramSize-1] = 0x0000;
}
/****************************************************************/
//move display to the right
//
uint8_t HT16K33::moveRight(void)
{
    shiftRight();
    return sendLed();
}
/****************************************************************/
//move display to the left
//
uint8_t HT16K33::moveLeft(void)
{
    shiftLeft();
    return sendLed();
}
/****************************************************************/
//left justified printing, shift right
//
uint8_t HT16K33::alphaPrintL(char value)
{
    uint8_t i = 0;
    shiftRight();
    i|= alphaSet(0, value);
    i|= sendLed();
    return i;
}
/****************************************************************/
//right justified printing, shift left
//
uint8_t HT16K33::alphaPrintR(char value)
{
    uint8_t i = 0;
    shiftLeft();
    i|= alphaSet(_ramSize-1, value);
    i|= sendLed();
    return i;
}
/****************************************************************/
//left scrolling string with delay
//
uint8_t HT16K33::alphaPrintSL(std::string value, int _delay)
{
    uint8_t i = 0;
    int j = 0;
    while(j < (int)value.length())
    {
        i|=alphaPrintR(value[j++]);
        delay(_delay);
    }
    return i;
}
/****************************************************************/
//right scrolling string with delay
//
uint8_t HT16K33::alphaPrintSR(std::string value, int _delay)
{
    uint8_t i = 0;
    int j = value.length();
    while(j > 0)
    {
        i|=alphaPrintL(value[--j]);
        delay(_delay);
    }
    return i;
}
