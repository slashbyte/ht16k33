# HT16K33 for the PI

Development library for use with the Holtek HT16K33

## Getting Started

To get things up and running on the Pi, you will need a few things.

### Enable i2c

To enable i2c on the Pi

[More I2c Info](https://diyprojects.io/activate-i2c-bus-raspberry-pi-3-zero/)
```
sudo raspi-config
```
Select option 5 and enable i2c
```
sudo reboot
```

### Installing Other Stuff

[Even More I2c info](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c)
```
sudo apt-get update
sudo apt-get install -y i2c-tools
```

Ensure i2c is working, check with...
```
sudo i2cdetect -y 0
sudo i2cdetect -y 1
```

## Running the Demo

If everything is setup, run "make" in the src directory

```
make
./demo
```
![GitHub Logo](/pi/pic/demo.jpg)