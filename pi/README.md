# HT16K33 for the PI

This library is used to allow an arduino like platform (now with PI!) to communicate with the holtek ht16k33 chip
to do things like turn on/off LEDs or scan keys

## Getting Started

To get things up and running on the Pi, you will need a few things.

### Enable i2c

To enable i2c on the Pi
[More I2c info]: https://diyprojects.io/activate-i2c-bus-raspberry-pi-3-zero/
```
sudo raspi-config
```
Select option 5 and enable i2c
```
sudo reboot
```

### Installing wiringPi

To install the wiringPi library
[WiringPi library info]: http://wiringpi.com/download-and-install/
```
sudo apt-get update
sudo apt-get install wiringpi
```

## Running the Demo

If everything is setup, run "make" in the demo directory

```
make
./demo
```
![GitHub Logo](/pi/pic/demo.jpg)