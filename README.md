# WhyzaGC
Feather Huzzah ESP8266/ESP32 v2 + Featherwing 128x64 OLED Arduino addon to MightyOhm Gieger Counter
---
permlink: /README.md/
title: README
---

This project is about adding a wireless Adafruit Arduino Feather HUZZAH ESP8266 or ESP32 v2 with Adafruit Featherwing 128x64 OLED to interface with the self-assembled MightOhm Geiger Kit.

![gc](https://user-images.githubusercontent.com/109115488/183527736-65f04142-ec97-42df-9d83-001d34401015.jpg)

## Introduction:

I am always on the lookout for unique and interesting kits to build which led to me discovering the MightOhm Geiger kit. 
While there are a couple of other geiger kits around, the fact that the MightOhm was in stock ( rare in these post covid times! ), it included a commonly available detector tube ( for relative data comparisons ), its serial output, along with its steampunk looks, it had me sold. 

While waiting for delivery I checked a number of forums looking for ideas on how to interface with the counter for both the Raspberry Pi and Arduino. I found a dated project using the Huzzah and MightyOhm and after researching parts availability, I decided to do my own Arduino project from scratch as I wanted to increase my Arduino coding skills. I also found some good ideas for interfacing to the Raspberry Pi which I also cover.

I initially started with the ESP8266 and then 'upgraded' to the ESP32 to take advantage of the dual core as I ran into limitations multitasking on the ESP8266 as discussed below. 

As such I would recommend the Feather Huzzah ESP32 v2 over the original ESP8266 version due to both the NeoPixel RGB LED and dual core functionality.

I also discovered Radmon.org which seemed a worthwhile project to contribute data to. It has a very simple and easy registration process...and runs on solar power !

I bought from ebay a piece of vaseline glass to test out the meter's functionality.

It is recommended to read all of this document before commencing the project.

## Features:

Adafruit Feather Huzzah ESP8266 and ESP32 v2 support, OLED display, Li-Po battery

Button inputs for 4 OLED display modes showing different data formats.
Includes bar meter strength graphs, auto scaling histogram and detailed data. Photos below.

Radmon.org data upload every 60 secs

https://radmon.org/index.php

Web server diagnostics available via HTTP over port 80 ( data, radmon.org upload status, debug, uptime )

Blue LED/Neopixel heartbeat, Red LED CPS ( Counts per Second )

Monitoring of MightOhm's serial data for disconnections.

MightyOhm serial data over HTTP and Telnet

mDNS multicast name registration

Neopixel radmon status indications. 

OTA Software upgrades via WiFi

NTP client for OLED and web server time display

Unix syslog status messages to your syslog server

## Parts list

MightOhm Gieger
https://mightyohm.com/blog/products/geiger-counter/

plus either Feather Huzzah ESP8266
https://www.adafruit.com/product/2821

or Feather Huzzah ESP32 v2
https://www.adafruit.com/product/3405

with Featherwing 128x64 OLED
https://www.adafruit.com/product/4650

Stacking Headers for Feather - 12-pin and 16-pin female headers
https://www.adafruit.com/product/2830

Micro USB (Huzzah ESP8266) or USB C (Huzzah ESP32 v2) cable to connect your PC to the Feather

Li-Po battery to replace the original AAA battery

Double sided tape.

Female breadboard jumper wires to connect to Raspberry Pi if desired.

Vaseline or uranium glass as a source of radiation.

## Assembly

While assembling your MightOhm if you are yet to do so, do not install the AAA battery holder as this is where the Feather Huzzah is to be placed. If you have assembled your kit, you will need to remove the battery holder.

Start by soldering the stacking headers to the Huzzah and the supplied headers on the Featherwing OLED as documented at the Adafruit's online guide. 

Then connect to your PC via the USB cable and you should see the Huzzah fire up and show serial output @ 115200 baud.  You should be able to then load and run sample code from the Arduino IDE under File -> Examples to verify hardware functionality, such as Basics -> Blink or WiFi -> WiFiScan.

Next we need to connect the MightOhm Geiger to the Huzzah. You can use a breadboard to test your setup if desired or you can simply dive in and solder the required 3 wires between the Huzzah and the Mighyohm. 

With the rear of the MightOhm accessible, the first is the MightOhm geiger serial TX pin which is found on J7 pin 4 on the mightyOhm which is connected to GPIO 13 on the Huzzah ESP8266 and GPIO 27 on the Huzzah ESP32 via the bottom of the board. This is pin 6 on the top from left to right ( from the front! ) on both Huzzah versions and is the white wire shown below.

Secondly connect the Huzzah's 3.3v and GND pins to the MightOhm's battery connections which are the red and black wires below.....Refer to pinout images at the end of this document, or Adafruit's pinout images online.

![underside](https://user-images.githubusercontent.com/109115488/183534736-6471d7e9-a969-49c6-ac1c-33ec735b5cfa.jpg)

You will notice in the above photo I have removed the stacking header pins on the bottom of the Huzzah but have not cut off the 1 pin for the VBUS/USB connection. I have bent it at a right angle so I can connect a 5v supply to power the Feather Huzzah (and charge the Li-Po battery).

Note that when the Huzzah is mounted in the MightOhm, access is lost to the onboard USB serial connection so you will need to upload the software mentioned below before final assembly. Once the software is loaded, you can use the OTA WiFi update feature if required when the Huzzah is mounted in it's final position in the MightyOhm Geiger for upgrades. 

After verifying the software below and the hardware is functional by connecting the Li-Po battery and checking operation, only then apply the double sided tape and secure the Feather Huzzah to the MightyOhm AAA battery space, and the Li-Po battery to the rear side of the MightyOhm.

## Software:

Tested on Arduino IDE 1.8.19/2.0.0-rc9.1 and ESP8266 Boards 3.0.2/ESP32 2.0.4 Arduino with Ubuntu Linux desktop 20.04 with both Feather Huzzah ESP8266 and Feather Huzzah ESP32 v2

The code is a result of learning many new skills along the way from various examples and online searches. It in essence reads the serial data from the MightyOhm using softSerial via a GPIO pin, displays this data in various ways on the OLED, and uses movingAvg to average the CPM ( Count per Minute ) over 1 minute and subsequently upload the result to radmon.org via a HTTP client GET. The histogram graph utilises the circularBuffer library which made the  graphing relatively easy. The code is broken up into individual functions so if you want to see how one of the graphs work it should be straightforward.

Download the correct ino sketch file for your hardware and check the list of libraries included in the sketch and ensure they are all installed.
If I recall correctly all libraries are available via the Arduino Library Manager.

Software available at:
https://github.com/Whyzanet/WhyzaGC

At the top of the sketch are the variables that will need changing for your specific environment.
Register with radmon.org first so you have all your required credentials. 

Upload the sketch and see if there are any missing libraries you need to fix..

Once the sketch is compiled and uploaded, input is via 5 featherwing button combinations. Button A, Button B, Button C, Button A&B ( default startup mode ) and finally Button B&C to disable and enable Wifi. See photos below of different display outputs. I found the Featherwing OLED buttons small enough and close enough to be able to use multiple button presses easily with my thumb (A&B and B&C for a total of 5)

HTTP diagnostic data is available on port 80 via URL

http://whyzagc-esp.local

Monitor easily via the following linux terminal command.

watch -n 10 curl -s whyzagc-esp.local

USB serial port is set to 9600 baud and will display the MightyOhm Gieger serial output ( when accessible )

To monitor the MightyOhm serial data remotely use

http://whyzagc-esp.local:81

and in your favorite telnet program

telnet whyzagc-esp.local

Blue Heartbeat LED/Neopixel is triggered by an EOL from the Gieger serial output. Should flash 1 per second as a result. If Geiger serial EOL data is not detected, the heartbeat will stop.

Feather Huzzah ESP32 v2 Neopixel will flash purple every 60 secs when starting the radmon.org update and then will flash green when having completed a successful update, or red when aborting an unsuccessful update.

OTA upgrading of compiled sketch .bin via WiFi using the following URL.

http://whyzagc-esp.local/upload

To locate the correct binary for upload, within the Arduino IDE interface select Sketch -> Export Compiled Binary, which will drop the compiled binary in to the same location as the ino sketch file.

WiFi software updates and HTTP server diagnostics solve the issue of the USB serial port being inaccessible when the Huzzah is mounted in the MightOhm's original battery location, preventing USB serial port upgrades and monitoring.

You can Enable/Disable Wifi through buttons B&C for portable/low power use. Wifi disable will also disable radmon.org updates and will require a reset to nable radmon.org uploads if startup behavior is enabled. As such if you toggle wifi off and back on, you will have WiFi connectivity but radmon updates are disabled until next reset.

Vaseline glass is a useful item for background level radiation testing. The piece of Vaseline glass I have will cause a count of around 135 CPM when up close to the detector tube. Around 18 CPM is normal for background radiation with the SBM20 detector tube.

 The SBM20 detects limited range beta particle radiation and penetrating long range gamma electromagnetic radiation, but not short range alpha particle radiation, which is fine as you don’t want to be playing with hazardous alpha radiation sources anyway ( e.g. a smoke alarm’s Americium-241 ).  
 
The Vaseline glass’s uranium radiation is mostly stopped by aluminum, which fits the definition of being mostly beta radiation. 

In addition to giving off radiation, Vaseline Glass/Uranium is also fluorescent bright green under UV light.

## ESP8266/ESP32/OLED specific settings

Other than the expected differences in libraries for the ESP8266 & ESP32 ( ESP8266WiFi.h/WiFi.h and ESP8266WebServer.h/WebServer.h ), the 2nd LED differences ( Blue LED vs Neopixel RGB LED ) and pinout changes, the Feather Huzzah ESP32 CPU is also dual core.

As such in the ESP32 code I have set the radmon.org update function to utilse CPU0 rather than CPU1 which all other code runs on by default. 

This solves an issue on the ESP8266 platform due to it's single CPU. While performing the TCP connection setup and get request for the radmon.org update, the CPU is unavailable for other tasks. Since the typical TCP setup, data exchange and teardown takes at least 1.5 seconds in my environment ( and up to 4+ secs ), it means that the ESP8266 will miss at least 1 line of serial data from the MightyOhm. Simply put, it will miss one line of data for every second that the upload takes. This is not really a big issue as the code is reading the MightyOhm CPM ( Counts per Minute ) value which is averaged and uploading the resulting 1 minute rolling average CPM value that the Arduino calculates. So the impact of missing a couple of average values is negligible overall. It explains why the code is measuring the upload time and showing it in the web diagnostics. I did try schedulers and yield() without success on the ESP8266.....

This is not a problem on the ESP32 where the radmon.org function ( and the functions it calls ) is pinned to CPU0 while the default CPU1 is free to carry out other tasks such as not missing the grabbing of the next line of serial data from the MightyOhm ;)

The other consideration is that I can confirm that OLED screen burn in does occur with the default contrast and use over 1000 hours as noted on the Adafruit site, resulting in a contrast deviation as the datasheet explains it. As such I have now set the contrast to a minimum to preserve the screen. This setting is near the top of the ino file if you desire to change it. A screen with contrast deviation from burn in will always be not as bright ( or white as in this case ) as a new screen, with the same settings. The color temperature is different and the whole screen is affected. You can increase the contrast of a burnt screen to somewhat compensate, but this may make the problem worse depending on your setting and will not change the color temperature.

## Additional Connections:

I am connecting the VBUS/USB pin on the Huzzah to a Raspberry Pi 5V pin via the Pi GPIO header ( any 5v supply will suffice), as well as a ground connection between the two boards. Current is usually around 110 mA, though will peak around 300 mA when charging the Li-Po battery.

I am also connecting the Gieger serial TX pin (J7 pin 4) to the Raspberry Pi 4's additional serial Rx pins on uart 4 and 5 ( the default serial port is already in use ) by setting dtoverlay=uart4 and dtoverlay=uart5 in the Pi's /boot/config.txt file and then connecting the Gieger serial Tx pin to both pins 33 and 21. These additional serial ports are found under /dev/ttyAMA1 and /dev/ttyAMA2 under Raspberry Pi OS Bullseye.
On one port I have configured a bash script to perform radmon.org updates for use if I don't want to upload from the Feather Huzzah for whatever reason. The other port is used by a perl script that displays graphs using rrdtool.

See this link for the perl script. Read the whole page before starting.
https://mightyohm.com/forum/viewtopic.php?t=3504

Finally I also have the MightyOhm pulse pin ( J6 pin 2 ) connected to pin 35 on the Pi 4 so that a bash script can look for a rising edge on GPIO 19. I use this to log occurrences of high CPS's for when I need to reconfirm the randomness of the universe. 

MightOhm with connections to Raspberry Pi pictured below. Red is 5V supply, Orange is serial TX, Yellow is pulse, and brown is GND

![rpi-connections](https://user-images.githubusercontent.com/109115488/183542370-b1425d1e-df4b-43f3-ac72-6ed607074740.jpg)

Default bootup/Button A & B display

![gc](https://user-images.githubusercontent.com/109115488/183527736-65f04142-ec97-42df-9d83-001d34401015.jpg)

Button A display

![a](https://user-images.githubusercontent.com/109115488/183542943-c080e2a9-8f17-4930-82a1-04a2702ff9c8.jpg)

Button B display

![b](https://user-images.githubusercontent.com/109115488/183542952-a5c750ff-7dba-4f3e-952d-4732805959bd.jpg)

Button C display
![c](https://user-images.githubusercontent.com/109115488/183542965-d20e37c1-c1be-48f8-adb3-025e99516f57.jpg)

Adafruit Feather Huzzah ESP8266 pinout 

![FeatherHuzzahESP8266pinout](https://user-images.githubusercontent.com/109115488/183532848-67e19153-23b0-45d9-b821-0f4a29993b36.png)

Adafruit Feather Huzzah ESP32 v2 pinout 

![FeatherHuzzahESP32pinout](https://user-images.githubusercontent.com/109115488/183532850-095e769c-4976-42ac-ab86-83e7bfbfaed9.png)

## Latest Version

The latest version of this document and project is available via:

https://github.com/Whyzanet/WhyzaGC
