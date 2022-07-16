# WhyzaGC
Feather Huzzah ESP8266/ESP32 v2 + Featherwing 128x64 OLED Arduino addon to MightyOhm Gieger Counter
---
permlink: /README.md/
title: README
---

Arduino code for the Adafruit Feather HUZZAH ESP8266 & ESP32 v2 with Adafruit Featherwing 128x64 OLED to interface with the MightyOhm Gieger Counter

https://mightyohm.com/blog/products/geiger-counter/

plus either

https://www.adafruit.com/product/2821

or

https://www.adafruit.com/product/3405

with 

https://www.adafruit.com/product/4650

## Description:

Inspired by Dan's post. Please read in full as there is vital information there that I have not duplicated.

http://syncchannel.blogspot.com/2016/01/wi-fi-and-oled-upgrade-for-mightyohm.html

I found that the post was quite old and would not compile without significant effort so I decided that a rewrite from scratch would be best to not only gain OLED functionality with the Mightohm, but also a few other features I desired. 

It seemed also a good opportunity to upgrade to the latest hardware so I utilised the Featherwing OLED 128x64 version which you will need if you are to use this code as I not only make use of all the available screen real estate, the 128 x 64 OLED also uses a different driver to the older 128 x 32 OLED.

## Features:

Feather Huzzah ESP8266 and ESP32 v2 support

Button inputs for 4 display modes showing different data formats and for disabling/enabling wifi. Includes simple meter strength graphs and auto scaling histogram. I found the Featherwing OLED buttons small enough and close enough to be able to use multiple button presses easily with my thumb ( A&B and B&C for a total of 5 )

radmon.org data upload every 60 secs

https://radmon.org/index.php

Web server diagnostics  available via HTTP over port 80 ( data, radmon.org upload status, debug, uptime )

NTP client for OLED and web server time display

Remote syslog messages to your syslog server

Blue/Neopixel LED heartbeat, Red LED CPS

Monitoring of MightOhm's serial data for disconnections.

Enable/Disable Wifi through buttons B&C for portable/low power use. Wifi disable will also disable radmon.org updates and will require a reset to renable radmon.org uploads if enabled. As such if you toggle wifi off and back on, you will have WiFi connectivity but radmon updates are disabled until next reset.

## How to use:

Download the correct ino file and upload to either your Feather Huzzah ESP8266 or Feather Huzzah ESP32 v2. Check the list of libraries included in the ino file and ensure they are all installed.
If I recall correctly all libraries are available via the Arduino Library Manager.

Tested on Arduino IDE 1.8.19 and ESP8266Boards 3.0.2/ESP32 Arduino on Ubuntu Linux desktop 20.04 with both Feather Huzzah ESP8266 and Feather Huzzah ESP32 v2

At the top of the ino file are the variables that will need changing for your specific environment.

Input is via 5 featherwing button combinations. Button A, Button B, Button C, Button A&B ( default startup mode ) and finally Button B&C to disable and renable Wifi.

HTTP diagnostic data is available via the ESP8266/ESP32 IP address on port 80. Huzzah IP address is shown via Button A display.

Monitor easily via the following linux command or your web browser

watch -n 10 curl -s 192.168.0.x

where 192.168.0.x is replaced by your Huzzah IP address.

USB serial port is set to 9600 baud and will display the MightyOhm Gieger serial output.

Blue Heartbeat LED is triggered by an EOL from the Gieger serial output. Should flash 1 per second as a result. If Geiger serial EOL data is not detected, heartbeat will stop.

Note: I am no coder expert and have basically fumbled my way through getting something functional on what is one of my first Arduino projects. I am sure the code could be a lot more efficient and improved upon which would be good to see if someone has the inclination. Meanwhile it is what it is.....and I hope if gives others guidance in achieving whatever you are looking for.

## ESP8266/ESP32/OLED software differences

Other than the expected differences in libraries ( ESP8266WiFi.h/WiFi.h and ESP8266WebServer.h/WebServer.h ), hardware ( Blue LED vs Neopixel RGB LED ) and pinouts, the Feather Huzzah ESP32 CPU is also dual core.

As such in the ESP32 code I have set the radmon.org update function to use CPU0 rather than CPU1 which everything runs on by default. 

This solves an issue on the ESP8266 platform due to it's single CPU. While performing the TCP connection setup and get request for the radmon.org update, the CPU is unavailable for other tasks. Since the typical TCP setup, data exchange and teardown takes at least 1.5 seconds in my environment ( and up to 4+ secs ), it means that the ESP8266 will miss at least 1 line of serial data from the MightyOhm. Simply put, it will miss one line of data for every second that the upload takes. This is not really a big issue as the code is reading the MightyOhm CPM value which is averaged and uploading the resulting 1 minute rolling average CPM value that the Arduino calculates. So the impact is negligible. It explains why the code is measuring the upload time and showing it in the web diagnostics. I did try schedulers without success on the ESP8266.....

This is not a problem on the ESP32 where the radmon.org function ( and the functions it calls ) is pinned to CPU0 while the default CPU1 is free to carry out other tasks such as not missing the grabbing of the next line of serial data from the MightyOhm ;)

The other difference between the ESP8266 and the ESP32 code is that the ESP32 Neopixel every 60 secs will flash purple when starting the radmon.org update and then flash green when having completed a successful update, or flash red for an unsuccessful update.

The other consideration is that I can confirm that OLED screen burn in does occur with the default contrast and use over 1000 hours as noted on the Adafruit site, resulting in a contrast deviation as the datasheet explains it. As such I have now set the contrast to a minimum to preserve the screen. This setting is near the top of the ino file if you desire to change it. A screen with contrast deviation from burn in will always be not as bright ( or white as in this case ) as a new screen, with the same settings. The color temperature is different and the whole screen is affected. You can increase the contrast of a burnt screen to somewhat compensate, but this may make the problem worse depending on your setting and will not change the color temperature.

## Hardware setup:

I have basically followed Dan's hardware setup with a few mods.

### Feather Huzzah ESP8266

I have connected the MightOhm geiger serial TX pin to GPIO 13 ( softSerial RX ) on the Huzzah ESP8266 as Dan also did. This is pin 6 on the top from left to right.

In addition, I wanted USB serial connectivity when the display was mounted in the MightyOhm for updates, so I have connected an Arduino usb/serial board to the Huzzah's RX and TX, along with RTS to pin En ( 2nd on top from left to right), and DTR to GPIO pin 0 ( pin 8 on top from left to right ).
You will need a usb/serial board that exposes the DTR pin for you to use to enable automated code uploading. Through this additional usb/serial board I am able to upload code to the ESP8266 without performing a manual reset. The plan is to mount it sideways using hot glue at the end of the Feather ESP8266 for rear access on the MightyOhm.

I am powering the Feather ESP8266 through the USB pin ( 3rd from top left to right ) from a Raspberry Pi 4 5V pin on the Pi GPIO header ( as well as a ground connection between the two ). Max current is usually around 110 mA, though will be higher when charging the Li battery.
I am also running the Gieger serial TX pin to the Raspberry Pi 4's additional serial Rx pins on uart 4 and 5 ( the default serial port is already in use ) by setting dtoverlay=uart4 and dtoverlay=uart5 in the Pi's /boot/config.txt file and then connecting the Gieger serial Tx pin to both pins 33 and 21. These additional serial ports are found under /dev/ttyAMA1 and /dev/ttyAMA2 under Raspberry Pi OS Bullseye.
On one port I have configured a bash script to perform radmon.org updates for use if I don't want to upload from the Feather Huzzah for whatever reason. The other port is used by a perl script that displays graphs using rrdtool.

See this link for the perl script. Read the whole page before starting.
https://mightyohm.com/forum/viewtopic.php?t=3504

I am powering the MightyOhm Geiger via the 3V3 and the GND pins on the HUZZAH connecting to the J6 pulse header connections on the MightyOhm.

Finally I also have the MightyOhm pulse pin connected to pin 35 on the Pi 4 so that a bash script can look for a rising edge on GPIO 19. I use this to log rare occurences of high CPS's when I am interested.

### Feather Huzzah ESP32 v2

With the differences between the ESP2866 and ESP32 pinouts, I have connected the MightOhm geiger serial TX pin to GPIO 27 ( softSerial RX ) on the Huzzah ESP32 v2. This is pin 6 on the top from left to right on the ESP32.

## Latest Version

The latest version of this code is available via:

https://github.com/Whyzanet/WhyzaGC
