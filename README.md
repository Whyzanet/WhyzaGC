# WhyzaGC
Feather Huzzah ESP8266/ESP32 v2 + Featherwing 128x64 OLED Arduino addon to MightyOhm Gieger Counter
---
permlink: /README.md/
title: README
---

This project is about building your own wireless Adafruit Arduino Feather HUZZAH ESP8266 or ESP32 v2 with Adafruit Featherwing 128x64 OLED addon to interface with the self-assembled MightyOhm Geiger Kit.

![gc](https://user-images.githubusercontent.com/109115488/183527736-65f04142-ec97-42df-9d83-001d34401015.jpg)

## Introduction:

I am always on the lookout for unique and interesting kits to build which led to me discovering the MightyOhm Geiger kit.

While there are a couple of other geiger kits around, the fact that the MightyOhm was in stock ( rare in these post covid times! ), it included a commonly used detector tube ( for relative data comparisons ), its serial output, along with its steampunk looks, it had me sold. 

I initially started with the ESP8266 and then 'upgraded' to the ESP32 to take advantage of the dual core as I ran into limitations multitasking on the ESP8266 as discussed below. 

As such I would recommend the Feather Huzzah ESP32 v2 over the original ESP8266 version due to both the NeoPixel RGB LED and dual core functionality. That said, if you have a Feather Huzzah ESP8266 already, it will work just fine.

I also discovered Radmon.org which seemed a worthwhile project to contribute data to. It has a very simple and easy registration process...and runs on solar power !

While waiting for delivery I checked a number of forums looking for ideas on how to interface with the counter for both the Raspberry Pi and Arduino platforms. I found a dated project using the Feather Huzzah ESP8266 and the MightyOhm for inspiration but it was not going to compile on the current libraries, so I decided to do my own Arduino project from scratch as I also wanted to increase my Arduino coding skills. I also found some good ideas for interfacing to the Raspberry Pi which I also cover.

It was also a good opportunity to learn a lot more about radiation and our environment. 

The MightyOhm measures background radiation levels, which Wikipedia defines as “Background radiation originates from a variety of sources, both natural and artificial. These include both cosmic radiation and environmental radioactivity from naturally occurring radioactive materials (such as radon and radium), as well as man-made medical X-rays, fallout from nuclear weapons testing and nuclear accidents.”

I bought a number of items to test out the meter's functionality which are discussed below. 

It is recommended to read all of this document before commencing the project.

## Features:

Adafruit Feather Huzzah ESP8266 and ESP32 v2 support, OLED display, Li-Po battery

Button inputs select between 3 OLED data display modes showing different formats.
Including bar meter strength graphs, detailed data, plus a auto scaling histogram. Photos below.

Radmon.org data upload every 60 secs

https://radmon.org/index.php

Web server diagnostics available via HTTP over port 80 ( data, radmon.org upload status, debug, uptime )

Blue LED/Neopixel heartbeat, Red LED CPS ( for Counts per Second <= 5 )

Monitoring of MightyOhm's serial data for disconnections.

MightyOhm serial data over HTTP and Telnet

mDNS multicast name registration

Neopixel radmon status feedback. 

OTA Software upgrades via WiFi

NTP client for OLED and web server time display

Unix syslog status messages to your syslog server

## Parts list

MightyOhm Gieger
https://mightyohm.com/blog/products/geiger-counter/
https://www.adafruit.com/products/483

plus either Feather Huzzah ESP8266
https://www.adafruit.com/product/2821

or Feather Huzzah ESP32 v2
https://www.adafruit.com/product/3405

with Featherwing 128x64 OLED
https://www.adafruit.com/product/4650

Stacking Headers for Feather - 12-pin and 16-pin female headers
https://www.adafruit.com/product/2830

Micro USB (Huzzah ESP8266) or USB C (Huzzah ESP32 v2) cable to connect your PC to the Feather

Li-Po battery to replace the original AAA batteries

Double sided tape.

Female breadboard jumper wires to connect to Raspberry Pi if desired.

A test source of radiation, discussed in Testing below.

## Assembly

While assembling your MightyOhm if you are yet to do so, do not install the AAA battery holder as this is where the Feather Huzzah is to be placed. If you have assembled your kit, you will need to remove the battery holder.

Start by soldering the stacking headers to the Huzzah and the supplied headers on the Featherwing OLED as documented at the Adafruit's online guide for your hardware. Familiarise yourself with all the documentation.

Then connect to your PC via the USB cable and you should see the Huzzah fire up and show serial output @ 115200 baud. After selecting the correct board, you should be able to then load and run sample code from the Arduino IDE under File -> Examples to verify hardware functionality, such as Basics -> Blink or WiFi -> WiFiScan.

Next we need to connect the MightyOhm Geiger to the Huzzah. You can use a breadboard to test your setup if desired or you can simply dive in and solder the required 3 wires between the Huzzah and the MightyOhm. 

With the rear of the MightyOhm accessible, the first is the MightyOhm geiger serial TX pin which is found on J7 pin 4 on the MightyOhm which is connected to GPIO 13 on the Huzzah ESP8266 and GPIO 27 on the Huzzah ESP32 via the bottom of the board. This is pin 6 on the top from left to right ( from the front! ) on both Huzzah versions and is the white wire shown below.

Secondly connect the Huzzah's 3.3v and GND pins to the MightyOhm's battery connections which are the red and black wires below.....Refer to pinout images at the end of this document, or Adafruit's pinout images online.

![underside](https://user-images.githubusercontent.com/109115488/183534736-6471d7e9-a969-49c6-ac1c-33ec735b5cfa.jpg)

You will notice in the above photo I have removed the stacking header pins on the bottom of the Huzzah but have not cut off the 1 pin for the VBUS/USB connection. I have bent it at a right angle so I can connect a 5v supply to power the Feather Huzzah (and charge the Li-Po battery).

Note that when the Huzzah is mounted in the MightyOhm, access is lost to the onboard USB serial connection so you will need to upload the software mentioned below before final assembly. Once the software is loaded, you can use the OTA WiFi update feature if required when the Huzzah is mounted in its final position in the MightyOhm Geiger for upgrades. 

After verifying the software below and the hardware is functional by connecting the Li-Po battery and checking operation, only then apply the double sided tape and secure the Feather Huzzah to the MightyOhm AAA battery space, and the Li-Po battery to the rear side of the MightyOhm.

## Software:

Tested on Arduino IDE 1.8.19/2.0.0rc9.1 and ESP8266 Boards 3.0.2/ESP32 2.0.4 Arduino with Ubuntu Linux desktop 20.04 with both the Feather Huzzah ESP8266 and Feather Huzzah ESP32 v2.

Arduino software for ESP8266 and ESP32 versions are available at:

https://github.com/Whyzanet/WhyzaGC

Download the correct ino sketch file for your hardware, either ESP8266 or ESP32. 

Note: The Feather HUzzah ESP8266 will require the CPU Frequency set to 160MHz to accomodate high values of CPS/CPM.

At the top of the sketch are the variables that will need changing for your specific environment. Register with radmon.org first so you have your required credentials.

The following libraries are available via the Arduino Library Manager and will need to be installed:

ESPSoftwareSerial, Tlog, NTPClient, Adafruit_GFX, Adafruit_SH110X, Adafruit Feather OLED, Adafrui NeoPixel ( ESP32 ), Uptime Library, CircularBuffer.

The following libraries will need to be downloaded to your library directory:

movingAvg:

https://github.com/JChristensen/movingAvg

Syslog:

https://github.com/arcao/Syslog

ESP8266mDNS ( for ESP8266 ):

https://github.com/LaborEtArs/ESP8266mDNS

The code in essence reads the serial data from the MightyOhm using SoftwareSerial via a GPIO pin, displays this data in various ways on the OLED, and uses movingAvg to average the CPM ( Count per Minute ) over 1 minute and subsequently upload the result to radmon.org via a HTTP client GET.

The histogram graph utilises the circularBuffer library which made the graphing relatively easy.

There are a number of networking functions which should be self explanatory ( mDNS, Web server, NTP client, serial to telnet/HTTP, unix syslog, OTA upgrades ).

The code is broken up into individual functions so if you want to see how one of the functions work it should be straightforward.

Below is a list of the main functions:

wifion - Activate WiFi, setup Web server, OTA upgrades and NTP client

wifioff - deactivate WiFi and radmon updates

serialmon - Monitor the MightyOhm serial connection for disconnects longer than 15 seconds

grab - Read the MightyOhm serial data via SoftwareSerial GPIO RX pin

process - Check for valid MightyOhm EOL serial data, flash the heartbeat LED, extract the required data for variables, averages and histogram display.

grabgraphdata - Grab current data for histogram circularBuffer

averagedata - average CPM data for radmon update

radmon - Once per minute flash the NeoPixel purple ( ESP32), perform the HTTP Get with radmon.org, flash the NeoPixel ( ESP32 ) red or green on the result.

handleRootPath - configure the web diagnostics on the HTTP server

logstats - send Unix syslog messages each hour

buttons - Check for button input 

smallData - Display all data available in a small size font

mediumData - Display all Geiger Counter Data in a medium size font

largeData - Display mimimal CPS/CPM data in a large font size

histogramData - Display histogram graph

runntp - Update NTP time at update interval

Upload the sketch and check for any missing libraries you may need.

Once running, input is via the 3 OLED Featherwing buttons. Button A and Button C will cycle up and down respectively through the 3 available data display modes. Button B selects histogram display mode. See photos below of different display outputs.

Buttons B & C together will toggle WiFi disable and enable, which I found easy to select as the buttons are small enough and so close together. WiFi disable will also disable radmon.org updates and will require a reset to enable radmon.org uploads if radmon startup behavior is enabled. As such if you toggle WiFi off and back on, you will have WiFi connectivity but radmon updates are disabled until the next reset....useful for testing, as per below.

USB serial port is set to 115200 baud and will display the MightyOhm Gieger serial output ( when not mounted in kit and accessible ).

HTTP diagnostic data is available on port 80 via URL

http://whyzagc-esp.local

Monitor easily via the following linux terminal command.

watch -n 10 curl -s whyzagc-esp.local

To monitor the MightyOhm serial data remotely use

http://whyzagc-esp.local:81

and in your favorite telnet program

telnet whyzagc-esp.local

Blue Heartbeat LED/Neopixel is triggered by an EOL from the Gieger serial output. Should flash 1 per second as a result. If Geiger serial EOL data is not detected, the heartbeat will stop.

Feather Huzzah ESP32 v2 Neopixel will flash purple every 60 secs when starting the radmon.org update and then will flash green when having completed a successful update, or red when aborting an unsuccessful update.

OTA upgrading of compiled sketch .bin via WiFi using the following URL.

http://whyzagc-esp.local/upload

To locate the correct binary for upload, within the Arduino IDE interface select Sketch -> Export Compiled Binary, which will drop the compiled binary into the same location as the ino sketch file.

WiFi (OTA) software updates, HTTP server for diagnostics and MightyOhm serial port redirection over telnet/HTTP workaround limitations with the USB serial port being inaccessible when the Huzzah is mounted in the MightyOhm's original battery location, preventing USB serial port access, upgrades, serial diagnostics and MightyOhm serial port data access.

## Testing

There are a number of items you can collect to test radiation. 

The SBM20 tube detects limited range high energy beta particle radiation and penetrating long range gamma electromagnetic radiation, but not short range alpha particle radiation which is stopped by the tube's metallic outer composition, which is fine as you don’t want to be playing with hazardous alpha radiation sources anyway ( e.g. a smoke alarm’s Americium-241 ).  

Vaseline glass contains uranium which makes it a useful item for background level radiation testing and is easily available on Ebay. Other options are volcanic rock, granite benchtops, some types of Fiesta tableware, or chunks of uranium... http://unitednuclear.com/
 
The Vaseline glass’s radiation is mostly stopped by aluminum, which fits the definition of being mostly beta radiation. 

In addition to ( and independently of ) emitting radiation, Vaseline glass/uranium also glows a fluorescent bright green under UV light.

I also bought a ‘nano technology, ionizing anti EMF’ therapeutic volcanic pendant necklace and while it gives a relatively weak count among my samples of only 90+ CPM, that works out to about 4.5 mSieverts/year, which is 4.5 times the annual normal background exposure of 1 mSieverts/year here in Australia ( which is still very small ). Its glossy product documentation has a long list of varied health benefits ( including fighting cancer cells and protection from harmful 5G technology ) if worn on the skin due to its ‘scalar energy’.

I also picked up from United Nuclear a small speciman bottle of uranium ore chunks, a geiger counter test card and a Fiestaware glazed plate, the latter having the greatest radiation emissions detected among all the samples I have.

An airplane flight would also be an interesting test environment due to increased cosmic ( though less terrestrial ) radiation levels at altitude. Latitude also affects background cosmic radiation with higher levels the closer you are to the poles. Calculator available at https://jag.cami.jccbi.gov/

Measured Test Results in order of strength:

Background radiation: 18 CPM

Therapeutic volcanic pendant: 90 CPM

Vaseline glass: 173 CPM

United Nuclear small speciman bottle of uranium ore chunks: 1700 CPM

United Nuclear geiger counter test card: 2700 CPM

Fiesta tableware red/orange glazed plate: 6500 CPM

Maximum tested using a combination of the last 3 samples listed above: 208 CPS or 12500 CPM

## ESP8266/ESP32/OLED specific settings

As noted above, the Feather Huzzah ESP8266 will require the CPU Frequency set to 160MHz to accomodate high values of CPS/CPM.

Other than the expected differences in libraries for the ESP8266 & ESP32 retrospectively ( ESP8266WiFi.h/WiFi.h, ESP8266WebServer.h/WebServer.h and ESP8266mDNS/ESPmDNS ), the 2nd LED differences ( Blue LED vs Neopixel RGB LED ) and pinout changes, the Feather Huzzah ESP32 CPU is also dual core.

As such in the ESP32 code I have set the radmon.org update function to utilse CPU0 rather than CPU1 which all other code runs on by default. 

This solves an issue on the ESP8266 platform due to it's single CPU. While performing the TCP connection setup and get request for the radmon.org update, the CPU is unavailable for other tasks. Since the typical TCP setup, data exchange and teardown takes at least 1.5+ seconds in my environment ( and up to N  secs ! ), reulting in the ESP8266 missing N lines of serial data from the MightyOhm, one per second. This is not really a big issue as the code is reading the MightyOhm CPM ( Counts per Minute ) value which is itself averaged and uploading the resulting arduino calculated 1 minute rolling average. So the impact of missing a couple of average values is negligible overall. It explains why the code is measuring the upload time and showing it in the web diagnostics. I did try schedulers and yield() without success on the ESP8266.....

This is not a problem on the ESP32 where the radmon.org function ( and the functions it calls ) is pinned to CPU0 while the default CPU1 is free to carry out other tasks such as not missing the grabbing of the next line of serial data from the MightyOhm ;)

The other consideration is that I can confirm that OLED screen burn in does occur with the default contrast and use over 1000 hours as noted on the Adafruit site, resulting in a contrast deviation as the datasheet explains it. As such I have now set the contrast to a minimum to preserve the screen. This setting is near the top of the ino file if you desire to change it. A screen with contrast deviation from burn in will always be not as bright ( or white as in this case ) as a new screen, with the same settings. The color temperature is different. You can increase the contrast of a burnt screen to somewhat compensate, but this may make the problem worse depending on your setting and will not change the color temperature.

## Additional Connections:

I am connecting the VBUS/USB pin on the Huzzah to a Raspberry Pi 5V pin via the Pi GPIO header as well as a ground connection between the two boards. Current is usually around 110 mA, though will peak at around 300 mA when charging the Li-Po battery which probabaly means any 5v supply will suffice. 

I am also connecting the Gieger serial TX pin (J7 pin 4) to the Raspberry Pi 4's additional serial Rx pins on uart 4 and 5 ( the default serial port is already in use ) by setting dtoverlay=uart4 and dtoverlay=uart5 in the Pi's /boot/config.txt file and then connecting the Gieger serial Tx pin to both pins 33 and 21. These additional serial ports are found under /dev/ttyAMA1 and /dev/ttyAMA2 under Raspberry Pi OS Bullseye.
On one port I have configured a bash script to perform radmon.org updates for use if I don't want to upload from the Feather Huzzah for whatever reason. The other port is used by a perl script that displays graphs using rrdtool.

See this link for the perl script. Read the whole page before starting.
https://mightyohm.com/forum/viewtopic.php?t=3504

Finally I also have the MightyOhm pulse pin ( J6 pin 2 ) connected to pin 35 on the Pi 4 so that a bash script can look for a rising edge on GPIO 19. I use this to log occurrences of high CPS's for when I need to reconfirm the randomness of the universe. 

MightyOhm with connections to Raspberry Pi pictured below. Red is 5V supply, Orange is serial TX, Yellow is pulse, and brown is GND

![rpi-connections](https://user-images.githubusercontent.com/109115488/183542370-b1425d1e-df4b-43f3-ac72-6ed607074740.jpg)

Default bootup/histogram display mode

![gc](https://user-images.githubusercontent.com/109115488/183527736-65f04142-ec97-42df-9d83-001d34401015.jpg)

All data display mode

![a](https://user-images.githubusercontent.com/109115488/183542943-c080e2a9-8f17-4930-82a1-04a2702ff9c8.jpg)

Geiger data display mode

![b](https://user-images.githubusercontent.com/109115488/183542952-a5c750ff-7dba-4f3e-952d-4732805959bd.jpg)

CPS & CPM large display mode

![c](https://user-images.githubusercontent.com/109115488/183542965-d20e37c1-c1be-48f8-adb3-025e99516f57.jpg)

HTTP diagnostics:

![gc-diags](https://user-images.githubusercontent.com/109115488/184987250-8a6153ce-7e5f-45d1-94b2-17fc3f9517e0.png)

Telnet output of MightyOhm serial data

![gc-telnet](https://user-images.githubusercontent.com/109115488/184987262-78afafae-8e1f-4c8e-927b-bef9ad719035.png)

Vaseline Glass sourced from Ebay. In addition to ( and independently of ) emitting radiation, Vaseline glass/uranium also glows a fluorescent bright green under UV light.

![vaseline glass uv2](https://user-images.githubusercontent.com/109115488/184986286-c954eba3-c4ca-428d-ace4-f207543e72a7.jpg)

Fiesta tableware plate, therapeutic volcanic pendant, United Nuclear uranium ore chunks and geiger counter test card.

![Fiestaware-Testcard-Uorejpg](https://user-images.githubusercontent.com/109115488/189282549-54f30927-5ccf-40b2-9b11-2017fbfefbd7.jpg)

Raspberry Pi Graphs using rrdtool

![rpi-graph](https://user-images.githubusercontent.com/109115488/184986901-79bdcfbd-5aa8-444f-a940-c020fcd8be97.png)

Radmon Site

![Radmon-site](https://user-images.githubusercontent.com/109115488/184986769-9446e31d-5155-4078-8dea-657e5d715cdd.png)
 
Radmon data

![radmon-12hr](https://user-images.githubusercontent.com/109115488/184986777-30e85e87-a652-45a8-9987-b25e3880f064.png)

Adafruit Feather Huzzah ESP8266 pinout 

![FeatherHuzzahESP8266pinout](https://user-images.githubusercontent.com/109115488/183532848-67e19153-23b0-45d9-b821-0f4a29993b36.png)

Adafruit Feather Huzzah ESP32 v2 pinout 

![FeatherHuzzahESP32pinout](https://user-images.githubusercontent.com/109115488/183532850-095e769c-4976-42ac-ab86-83e7bfbfaed9.png)

## Latest Version

The latest version of this document and project is available via:

https://github.com/Whyzanet/WhyzaGC
