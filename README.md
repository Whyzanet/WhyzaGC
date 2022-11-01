# WhyzaGC
Feather Huzzah ESP8266/ESP32 v2 + Featherwing 128x64 OLED Arduino addon to MightyOhm Gieger Counter C++
---
permlink: /README.md/
title: README
---

This project is about building your own DIY wireless Adafruit Arduino Feather HUZZAH ESP8266 or ESP32 v2 microprocessor with Adafruit Featherwing 128x64 OLED display addon to interface with the self-assembled DIY MightyOhm Geiger Counter Kit. I also added a random number generator based on the radiation counts for fun.

![WhyzaGC-histogram1](https://user-images.githubusercontent.com/109115488/192123691-52e77460-48d9-4fd2-a95f-41f3cf3bae22.jpg)

## Introduction:

I am always on the lookout for unique and interesting kits to build which led to me discovering the MightyOhm Geiger kit.

While there are a couple of other geiger kits around, the fact that the MightyOhm was in stock ( rare in these post covid times! ), it included a commonly used detector tube ( for relative data comparisons ), its serial data output, along with its steampunk looks, it had me sold. 

I initially started with the ESP8266 and then 'upgraded' to the ESP32 to take advantage of the dual core as I ran into limitations multitasking on the ESP8266 as discussed below. 

As such I would recommend the Feather Huzzah ESP32 v2 over the original ESP8266 version due to both the NeoPixel RGB LED and dual core functionality. That said, if you have a Feather Huzzah ESP8266 already, it will work just fine.

I have packed as many software features in as I can think of in an attempt to learn as much as possible about the ESP software development.

I also discovered Radmon.org which seemed a worthwhile project to contribute data to. It has a very simple and easy registration process...and runs on solar power !

While waiting for delivery I checked a number of forums looking for ideas on how to interface with the counter for both the Raspberry Pi and Arduino platforms. I found a dated project using the Feather Huzzah ESP8266 and the MightyOhm for inspiration but it was not going to compile on the current libraries, so I decided to do my own Arduino project from scratch as I also wanted to increase my Arduino coding skills. I also found some good ideas for interfacing to the Raspberry Pi which I also cover.

It was also a good opportunity to learn a lot more about radiation and our environment. 

The MightyOhm measures background radiation levels, which Wikipedia defines as “Background radiation originates from a variety of sources, both natural and artificial. These include both cosmic radiation and environmental radioactivity from naturally occurring radioactive materials (such as radon and radium), as well as man-made medical X-rays, fallout from nuclear weapons testing and nuclear accidents.”

The most interesting explanation of the whole process I found at this link.

https://www.fourmilab.ch/hotbits/how3.html

I bought a number of items to test out the meter's functionality which are discussed below. 

It is recommended to read all of this document before commencing the project.

## Features:

Adafruit Feather Huzzah ESP8266 and ESP32 v2 support, OLED display, Li-Po battery

Button inputs select between 3 OLED data display modes showing different formats.
Including bar meter strength graphs, detailed data, plus an auto scaling histogram. Photos below.

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

Random number generator at rate of 1 character per two pulses detected.

Bootup animation logo

## Parts list

MightyOhm Geiger Counter DIY kit with GM tube and clear case
https://www.adafruit.com/products/483 and https://mightyohm.com/blog/products/geiger-counter/

plus either Feather Huzzah ESP8266
https://www.adafruit.com/product/2821

or Feather Huzzah ESP32 v2
https://www.adafruit.com/product/3405

with Featherwing 128x64 OLED
https://www.adafruit.com/product/4650

Stacking Headers for Feather - 12-pin and 16-pin female headers
https://www.adafruit.com/product/2830

Li-Po battery to replace the original AAA batteries
https://www.adafruit.com/product/258

Micro USB (Huzzah ESP8266) or USB C (Huzzah ESP32 v2) cable to connect your PC to the Feather

Double sided tape.

Female breadboard jumper wires to connect to Raspberry Pi if desired.

A test source of radiation, discussed in Testing below.

## Assembly

While assembling your MightyOhm if you are yet to do so, do not install the AAA battery holder as this is where the Feather Huzzah is to be placed. If you have assembled your kit, you will need to remove the battery holder.

Start by soldering the stacking headers to the Huzzah and the supplied headers on the Featherwing OLED as documented at the Adafruit's online guide for your hardware. Familiarise yourself with all the documentation.

Then connect to your PC via the USB cable and you should see the Huzzah fire up and show serial output @ 115200 baud in the Arduino IDE. After selecting the correct baud, you should be able to then load and run sample code under File -> Examples to verify hardware functionality, such as Basics -> Blink or WiFi -> WiFiScan.

Next we need to connect the MightyOhm Geiger to the Huzzah. You can use a breadboard to test your setup if desired or you can simply dive in and solder the required 3 wires between the Huzzah and the MightyOhm.  Optionally you can include a 4th wire if you would like to enable and play with the random number generator. I used a small gauge wire so I was able to feed the 4 wires through the circuit board hole under the battery cover.

With the rear of the MightyOhm accessible, the first wire to connect is the MightyOhm geiger serial TX pin which is found on J7 pin 4 on the MightyOhm which is connected to GPIO 13 on the Huzzah ESP8266 and GPIO 27 on the Huzzah ESP32 via the bottom of the board. This is pin 6 on the top from left to right ( from the front! ) on both Huzzah versions and is the blue wire shown below.

If you want to include the random number functionality, wire from the MightyOhm center pulse pin on J6 to GPIO 12. GPIO 12 is pin 5 next to pin 6 noted above on both Huzzah versions, and is the yellow wire in the image below.

Finally, connect the Huzzah's 3.3v and GND pins to the MightyOhm's battery connections which are the red and black wires below.....Refer to the images at the end of this document for both the Feather Huzzah's and the MightyOhm Gieger's header pinouts.

![WhyzaGC-underside wiring](https://user-images.githubusercontent.com/109115488/193185576-39bdc3c0-ae11-4c34-9da6-50260546a1f1.jpg)

You will notice in the above photo I have soldered a single stacking header pin on the bottom of the Huzzah on the VBUS/USB pin. I have soldered it at a right angle so I can connect Raspberry Pi 5v supply to power the Feather Huzzah (and charge the Li-Po battery) via a jumper cable from the Pi's header.

Note that when the Huzzah is mounted in the MightyOhm, access is lost to the onboard USB serial connection so you will need to upload the software mentioned below before final assembly. Once the software is loaded, you can use the OTA WiFi update feature if required when the Huzzah is mounted in its final position in the MightyOhm Geiger for upgrades.

After verifying the software below and the hardware is functional by connecting the Li-Po battery and checking operation, only then apply the double sided tape and secure the Feather Huzzah to the MightyOhm AAA battery space, and the Li-Po battery to the rear piece of the plastic case on the MightyOhm.

## Software:

Tested on Arduino IDE 1.8.19/2.0.0rc9.1 and ESP8266 Boards 3.0.2/ESP32 2.0.5 Arduino with Ubuntu Linux desktop 20.04 with both the Feather Huzzah ESP8266 and Feather Huzzah ESP32 v2.

Arduino C++ software for ESP8266 and ESP32 versions are available at:

Latest version available at https://github.com/Whyzanet/WhyzaGC

Download the correct C++ ino sketch file for your hardware, either ESP8266 or ESP32. 

Note: The Feather HUzzah ESP8266 will require the CPU Frequency set to 160MHz to accommodate high values of CPS/CPM.

At the top of the sketch are the variables that will need changing for your specific environment. Register with radmon.org first so you have your required credentials.

The following libraries are available via the Arduino Library Manager and will need to be installed.
I have listed the version that I tested under.

ESPSoftwareSerial(6.16.1), Tlog(1.03), NTPClient(3.2.1), Adafruit_GFX(1.11.1), Adafruit_SH110X(2.1.6), Adafruit Feather OLED(2.0.2), Adafrui NeoPixel ( ESP32 )(1.10.6), Uptime Library(1.0.0), CircularBuffer(1.3.3).

The following libraries will need to be downloaded to your library directory:

movingAvg(3.2.1):

https://github.com/JChristensen/movingAvg

Syslog(2.0.0):

https://github.com/arcao/Syslog

ESP8266mDNS(1.2) ( for ESP8266 ):

https://github.com/LaborEtArs/ESP8266mDNS

The code in essence reads the serial data from the MightyOhm using SoftwareSerial via a GPIO pin, displays this data in various ways on the OLED, and uses movingAvg to average the CPM ( Count per Minute ) over 1 minute and subsequently upload the result to radmon.org via a HTTP client GET.

The histogram graph utilises the circularBuffer library which made the graphing relatively easy.

There are a number of networking functions which should be self explanatory ( mDNS, Web server, NTP client, serial to telnet/HTTP, unix syslog, OTA upgrades ).

The random number generator uses the pulse signal from the MightyOhm to generate an  interrupt which will read the current value from a looping counter ( basically a roulette wheel ) that loops and counts from 0 to 15, which is then output as a hex value to serial/telnet/HTTP (Tlog). Collating and analysing this random output is discussed below.

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

largeData - Display minimal CPS/CPM data in a large font size

histogramData - Display histogram graph

runntp - Update NTP time at update interval

randomise - looping counter from 0 to 15 roulette wheel used for random number generation. Pinned to CPU0 on ESP32.

grabrandomnum - the function called by the interrupt to grab a number from the roulette wheel loop. Triggered by the pulse pin on the MightyOhm

echorandom - Display the random number on a new line via Tlog to serial/telnet/HTTP.

setup - Initialisation, Animate bitmap logo, start WiFi if enabled.

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

Feather Huzzah ESP32 v2 Neopixel will flash purple every 60 secs when starting the radmon.org update and then will flash green when having completed a successful update, or red when detecting an unsuccessful update.

OTA upgrading of compiled sketch .bin via WiFi using the following URL.

http://whyzagc-esp.local/upload

To locate the correct binary for upload, within the Arduino IDE interface select Sketch -> Export Compiled Binary, which will drop the compiled binary into the same location as the ino sketch file.

WiFi (OTA) software updates, HTTP server diagnostics and MightyOhm serial port redirection over telnet/HTTP all workaround limitations with the USB serial port being inaccessible when the Huzzah is mounted in the MightyOhm's original AA battery location, preventing USB serial port access to upgrades, diagnostics and MightyOhm raw data.

## Testing

There are a number of items you can collect to test radiation detection. 

The SBM20 tube detects limited range high energy beta particle radiation ( also known as electrons ) and penetrating long range gamma electromagnetic radiation ( also known as photons ), but not short range alpha particle radiation which is stopped by the tube's metallic outer composition, which is fine as you don’t want to be playing with hazardous alpha radiation sources anyway ( e.g. a smoke alarm’s Americium-241 ).  

Vaseline glass contains uranium which makes it a useful item for background level radiation testing and is easily available on Ebay. 
The Vaseline glass’s radiation is mostly stopped by aluminum, which fits the definition of being mostly beta radiation. 
In addition to ( and independently of ) emitting radiation, Vaseline glass/uranium also glows a fluorescent bright green under UV light.

Other testing options are volcanic rock, granite benchtops, some types of Fiesta tableware ( due to the colored glazing used ), or chunks of uranium.... http://unitednuclear.com/

I bought a ‘nano technology, ionizing anti EMF’ therapeutic volcanic pendant necklace and while it gives a relatively weak count among my samples of only 90+ CPM, that works out to about 4.5 mSieverts/year, which is 4.5 times the annual normal background exposure of 1 mSieverts/year here in Australia ( which is still very small ). Its glossy product documentation has a long list of varied health benefits ( including fighting cancer cells and protection from harmful 5G technology ) if worn on the skin due to its ‘scalar energy’.

I also picked up from United Nuclear a small specimen bottle of uranium ore chunks, a Cloud Chamber Source ( a bigger chunk of uranium ore ), a geiger counter test card and a Fiestaware glazed plate. The respective CPM readings are below.

Measured Test Results in order of strength:

Background radiation: 18 CPM

Therapeutic volcanic pendant: 90 CPM

Vaseline glass: 173 CPM

United Nuclear small specimen bottle of uranium ore chunks: 1700 CPM

United Nuclear geiger counter test card: 2700 CPM

No name tableware red/orange glazed plate: 6500 CPM

Fiesta tableware red/orange glazed plate: 9200 CPM

United Nuclear Cloud Chamber Source ( chunk of uranium ): 27300

Maximum tested using a combination of samples listed above: 865 CPS or 51900 CPM

An airplane flight would also be an interesting test environment due to increased cosmic radiation levels at altitude. Latitude also affects background cosmic radiation with higher levels the closer you are to the poles. Interesting calculator available at https://jag.cami.jccbi.gov/

## ESP8266/ESP32/OLED specific settings

As noted above, the Feather Huzzah ESP8266 will require the CPU Frequency set to 160MHz to accommodate high values of CPS/CPM.

Other than the expected differences in libraries for the ESP8266 & ESP32 retrospectively ( ESP8266WiFi.h/WiFi.h, ESP8266WebServer.h/WebServer.h and ESP8266mDNS/ESPmDNS ), the 2nd LED differences ( Blue LED vs Neopixel RGB LED ) and pinout changes, the Feather Huzzah ESP32 CPU is also dual core.

As such in the ESP32 code I have set both the radmon.org update and randomise functions to utilse CPU0 rather than CPU1, which all other code runs on by default. 

This solves 2 issue's on the ESP8266 platform due to it's single CPU. Firstly, while performing the TCP connection setup and get request for the radmon.org update, the (single) CPU is unavailable for other tasks. Since the typical TCP setup, data exchange and teardown takes at least 1.5+ seconds in my environment ( and up to N  secs ! ), resulting in the ESP8266 missing N lines of serial data from the MightyOhm, one per second. This is not really a big issue as the code is reading the MightyOhm CPM ( Counts per Minute ) value which is itself averaged and uploading the resulting arduino calculated 1 minute rolling average. So the impact of missing a couple of average values is negligible overall. It explains why the code is measuring the upload time and showing it in the web diagnostics. I did try schedulers and yield() without success on the ESP8266....

Secondly, because the roulette wheel function generating the random data is time critical, any sharing of CPU will corrupt the random data. My initial tests confirm this.

This is not a problem on the ESP32 where the radmon.org function ( and the functions it calls ) and the randomise function are both pinned to CPU0 while the default CPU1 is free to carry out other tasks. When not uploading to radmon, CPU0 is available to the randomise function 

The other hardware consideration is that I can confirm that OLED screen burn-in does occur with the default contrast and use over 1000 hours as noted on the Adafruit site, resulting in a contrast deviation as the datasheet explains it. As such I have now set the contrast to a minimum to preserve the screen. This setting is near the top of the ino file if you desire to change it.

## Random number generator.

Inspired by this project,

https://github.com/gbonacini/nuclear_random_number_generator

I added a simple random number generator, which seems to work well on the ESP32 platform.

You enable it via true or false at the top section of the ino file

bool randomon = true; // enable random number generator

It obviously takes  a bit of time to generate data if you use only background radiation levels as you only generate one random character per 2 pulses. I did testing with high counts utilising some of the radiation samples I have, and found the output data was far from random. I believe this is due to the SMB20 saturating and the associated dead zone when the tube is rendered insensitive.

The random numbers are available via the HTTP or telnet connection. A character is output on a new line for each pulse when enabled.

To analyse the data I installed ent via

apt-get install ent

I then downloaded the test directory from the nuclear_random_number_generator project above, cd to this test directory where you can run

./test_random_numbers.sh

to format and test the included rnd_nums.txt data.

running

ent -c rnd_nums.bin

will analyse the data per character while

ent -b -c rnd_nums.bin

will analyse the data per bit.

man ent

is your friend.

I collected random data from the Huzzah via the following command on my linux desktop.

nc whyzagc-esp.local 23 | tee -a rnd_nums.txt

and waited about 5 days until I had about 15M raw data which gave me around 50K of binary random data once filtered. 

I had to change line 6 in test_random_numbers.sh from

cat rnd_nums.txt | grep -v '^CPM' | tr -d '\r\n' | head -204800 > rnd_nums.clean.txt

to the rather convoluted

cat rnd_nums.txt | cut  -c -2 | grep --binary-files=text -v 'CP\|Te\|19\|C\|BP\|PS\|GP' | cut -c -1 | grep --binary-files=text '0\|1\|2\|3\|4\|5\|6\|7\|8\|9\|a\|b\|c\|d\|e\|f' | grep --binary-files=text -v -e '^$' | tr -d '\r\n' > rnd_nums.clean.txt

to ensure the filtered data collected via the nc command was free of the MightyOhm serial output and nc's output messages. You may need to modify this if your environment is different to mine ( eg the 19 in the search field is a match from the first two characters of 192.168.0.100, the address nc is connecting to ). This address is obviously not meant to be in the final random data.

Once you have run test_random_numbers.sh over your new data in rnd_nums.txt, you can analyze your data as per above with the ent command or visually with

./scatter.py rnd_nums.bin

I have initially found that the ESP8266 platform does not pass the Chi square distribution test under ent. This is no doubt due to the limitations of only one CPU where you can not dedicate the time critical randomise counter to a spare CPU. On the ESP32 hardware, I have pinned the randomise function to the previously underutilised CPU0. CPU0 was only performing the once per minute Radmon upload, so other than this upload which will pause the randomise function, it is available for dedicated use. 

## Additional Connections:

I am connecting the VBUS/USB pin on the Huzzah to a Raspberry Pi 5V pin via the Pi GPIO header as well as a ground connection between the two boards. Current is usually around 110 mA, though will peak at around 300 mA when charging the Li-Po battery which probably means any 5v supply will suffice. 

I am also connecting the Gieger serial TX pin (J7 pin 4) to the Raspberry Pi 4's additional serial Rx pins on uart 4 and 5 ( the default serial port is already in use ) by setting dtoverlay=uart4 and dtoverlay=uart5 in the Pi's /boot/config.txt file and then connecting the Gieger serial Tx pin to both pins 33 and 21. These additional serial ports are found under /dev/ttyAMA1 and /dev/ttyAMA2 under Raspberry Pi OS Bullseye.
On one port I have configured a bash script to perform radmon.org updates for use if I don't want to upload from the Feather Huzzah for whatever reason. The other port is used by a perl script that displays graphs using rrdtool.

See this link for the perl script. Read the whole page before starting.
https://mightyohm.com/forum/viewtopic.php?t=3504

Finally I also have the MightyOhm pulse pin ( J6 pin 2 ) connected to pin 35 on the Pi 4 so that a bash script can look for a rising edge on GPIO 19. I use this to log occurrences of high CPS's for when I need to reconfirm the randomness of the universe. 

MightyOhm with connections to Raspberry Pi pictured below. Red is 5V supply, Orange is serial TX, Yellow is pulse, and brown is GND

![WhyzaGC-rpi_wiring](https://user-images.githubusercontent.com/109115488/192123370-be4c7772-94e3-4354-a41b-df52f2c8204f.jpg)

Default bootup/histogram display mode

![WhyzaGC-histogram1](https://user-images.githubusercontent.com/109115488/192123691-52e77460-48d9-4fd2-a95f-41f3cf3bae22.jpg)

All data display mode while capturing a MightyOhm LED trigger

![WhyzaGC-alldata1](https://user-images.githubusercontent.com/109115488/192136285-4fa3d755-e4f2-479a-9ceb-c7b95cd33153.jpg)

Geiger data display mode 

![WhyzaGC-data](https://user-images.githubusercontent.com/109115488/192136369-a98f7c5c-eb16-41f6-889c-120d565889ee.jpg)

CPS & CPM large display mode

![WhyzaGC-largedata](https://user-images.githubusercontent.com/109115488/192123402-beeb0aa4-70b2-43d2-a367-72b785da4eda.jpg)

HTTP diagnostics:

![WhyzaGC-http-diags](https://user-images.githubusercontent.com/109115488/193431130-163e6753-30ea-4f36-9bd0-03043cd92357.png)

Telnet output of MightyOhm serial data with random number output enabled

![WhyzaGC-Telnet_random](https://user-images.githubusercontent.com/109115488/193193697-3d6ac85a-084c-4e04-a5d1-7e107b81b320.png)

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

MightyOhm Geiger schematic showing header pinout configurations.

![geiger_sch_fixedR5R6](https://user-images.githubusercontent.com/109115488/198948137-f1748bfc-4e6d-4b7e-b820-efd574d00066.png)

## Latest Version

The latest version of this document and project is available via:

https://github.com/Whyzanet/WhyzaGC
