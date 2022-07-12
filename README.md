# WhyzaGC
ESP2866 + OLED Arduino addon to Mightohm Gieger
---
permlink: /README.md/
title: README
---

Arduino code for the Adafruit Feather HUZZAH ESP8266 with Adafruit 128x64 OLED FeatherWing to interface with the MightyOhm Gieger Counter

https://mightyohm.com/blog/products/geiger-counter/

https://www.adafruit.com/product/2821

https://learn.adafruit.com/adafruit-128x64-oled-featherwing

## Description:

Inspired by Dan's post. Please read in full as there is vital information there that I have not duplicated.

http://syncchannel.blogspot.com/2016/01/wi-fi-and-oled-upgrade-for-mightyohm.html

I found that the post was quite old and decided I would rewrite from scratch the required code to not only gain OLED functionality with the Mightohm, but also a few other features I desired. I upgraded the Featherwing OLED to the 128x64 version which you will need if you are to use this code as I make use of all the available screen real estate.

## Features:

Button inputs for multiple display modes showing different data formats and for disabling wifi. Includes simple meter strength graphs and histogram. I found the buttons small enough and close enough to be able to use multiple button presses easily with my thumb ( A&B and B&C for a total of 5 )

Web server diagnostics  available on http port 80 ( Data, radmon.org upload status, Debug, uptime )

NTP client for OLED and web server time display

remote syslog messages to your syslog server

radmon.org data upload every 60 secs

Blue LED heartbeat, RED led CPS

Monitoring of MightOhm's serial data for disconnections.

Enable/Disable Wifi through buttons B&C for portable/low power use. Will disable radmon.org updates and requires a reboot to renable radmon.org uploads if enabled.

## How to use:

Download the ino file and upload to your Featherwing ESP8266. Check the list of libraries included in the ino file and ensure they are all installed.
If I recall correctly all libraries are available via the Arduino Library Manager.

Tested on Arduino IDE 1.8.19 and ESP8266Boards 3.0.2 on Ubuntu Linux desktop 20.04.

At the top of the ino file are the variables that will need changing for your specific enviroment.

Input is via 5 button combinations. Button A, Button B, Button C, Button A&B ( default startup mode ) and finally Button B&C to disable and renable Wifi.

Note: I am no coder expert and have basiclly fumbled my way through getting something functional on what is one of my first Arduino projects. I am sure the code could be a lot more effecient and improved upon which would be good to see if someone has the inclination. Meanwhile it is what it is.....and I hope if gives others guidance in achieving whatever you are looking for.

## Hardware considerations:

I have basically followed Dan's hardware setup with a few mods.

I have connected the MightOhm geiger serial TX pin to GPIO pin 13 on the Huzzah as Dan also did. This is pin 6 on the top from left to right.

In addition, I wanted USB serial connectivity when the display was mounted in the MightyOhm for updates, so I have connected an Arduino usb/serial board to the Huzzah's RX and TX, along with RTS to pin En ( 2nd on top from left to right), and DTR to GPIO pin 0 ( pin 8 on top from left to right ).
You will need a usb/serial board that exposes the DTR pin for you to use to enable automated code uploading. Through this additional usb/serial board I am able to upload code to the ESP8266 without performing a manual reset. The plan is to mount it sideways using hot glue at the end of the Feather ESP8266 for rear access on the MightyOhm.

I am powering the Feather ESP8266 through the USB pin ( 3rd from top left to right ) from a Raspberry Pi 4 5V pin on the Pi GPIO header ( as well as a ground connection between the two ). Max current is usually around 110 mA, though will max out when charging the Li battery.
I am also running the Gieger serial TX pin to the Raspberry Pi 4's additional serial Rx pins on uart 4 and 5 ( the default serial port is already in use ) by setting dtoverlay=uart4 and dtoverlay=uart5 in the Pi's /boot/config.txt file and then connecting the Gieger serial Tx pin to both pins 33 and 21. These additional serial ports are found under /dev/ttyAMA1 and /dev/ttyAMA2 under Raspberry Pi OS Bullseye.
On one port I have configured a bash script to perform radmon.org updates for use if I don't want to upload from the Feather Huzzah for whatever reason. The other port is used by a perl script that displays graphs using rrdtool.

See this link for the perl script. Read the whole page before starting.
https://mightyohm.com/forum/viewtopic.php?t=3504

I am powering the MightyOhm Geiger via the 3V3 and the GND pins on the HUZZAH connecting to the J6 pulse header connections on the MightyOhm.

Finally I also have the MightyOhm pulse pin connected to pin 35 on the Pi 4 so that a bash script can look for a rising edge on GPIO 19. I use this to log rare occurances of high CPS's when I am interested.

## Issues

The only issue outstanding is that when the ESP8266 is performing the TCP connection setup and get request for the radmon.org update, the single CPU is unavailable for other tasks. Since the typical TCP setup, data exchange and closing takes around 1.5 seconds in my enviroment, it means that the ESP8266 will miss 1 line of serial data from the MightyOhm. Simply it will miss one line of data for every second that the upload takes. This is not really a big issue as the unit is reading and uploading the CPM value, which is itself averaged by the MightyOhm. So the impact is negligble. It explains why the code is measuring the upload time and showing it in the web diagnostics. I did try schedulars without success.....when the CPU is busy...its busy. Nothing a schedular can do it seemed.

I have just received a Feather Huzzah 32 V2

https://www.adafruit.com/product/3405

My next project is to migrate my code to the newer ESP32 platform and see if the dual CPU resolves this issue.

