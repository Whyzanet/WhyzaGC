# WhyzaGC
Feather Huzzah ESP8266/ESP32 v2 + Featherwing 128x64 OLED Arduino addon to MightyOhm Geiger Counter C++
---
permlink: /README.md/
title: README
---

This project is about building your own DIY wireless Adafruit Arduino Feather HUZZAH ESP8266 or ESP32 v2 microprocessor with the Adafruit Featherwing 128x64 OLED display add-on to interface with the self-assembled DIY MightyOhm Geiger Counter Kit. Additionally, I have included an optional random number generator based on the random timing of the radiation counts for education and fun.

![WhyzaGC-histogram1](https://user-images.githubusercontent.com/109115488/192123691-52e77460-48d9-4fd2-a95f-41f3cf3bae22.jpg)

## Introduction:

I am always on the lookout for unique and interesting kits to build, which led me to discover the MightyOhm Geiger kit.

While there are a couple of other Geiger kits around, the fact that the MightyOhm included a commonly used detector tube (for relative data comparisons), had serial data output, and sported steampunk looks, had me sold.

Initially, I started with the ESP8266 and then 'upgraded' to the ESP32 to take advantage of the dual-core as I ran into limitations multitasking on the ESP8266, as discussed below.

As such, I would recommend the Feather Huzzah ESP32 v2 over the original ESP8266 version due to both the NeoPixel RGB LED and dual-core functionality. That said, if you already have a Feather Huzzah ESP8266, it will work just fine. However, I have released version 4 software only for the ESP32, which brings MQTT support.

I have packed as many software features as I can think of in an attempt to learn as much as possible about ESP software development.

I also discovered Radmon.org, which seemed like a worthwhile project to contribute data to. It has a very simple and easy registration process and runs on solar power! Your data will be retained by radmon.org for 2 years. If you donate your data will be retained for 5 years.

While waiting for delivery, I checked a number of forums looking for ideas on how to interface with the counter for both the Raspberry Pi and Arduino platforms. I found a dated project using the Feather Huzzah ESP8266 and the MightyOhm for inspiration, but it was not going to compile on the current libraries. So, I decided to do my own Arduino project from scratch as I also wanted to increase my Arduino coding skills. I also found some good ideas for interfacing with the Raspberry Pi, which I also cover.

It was also a good opportunity to learn a lot more about radiation, quantum physics, and our environment.

The MightyOhm measures background radiation levels, which Wikipedia defines as 'Background radiation originates from a variety of sources, both natural and artificial. These include both cosmic radiation and environmental radioactivity from naturally occurring radioactive materials (such as radon and radium), as well as man-made medical X-rays, fallout from nuclear weapons testing, and nuclear accidents.'

The most interesting explanation of the whole radioactive process and how you can use it in practical ways, I found, is discussed in this next link from a fascinating long-standing project now well and truly obsolete in this era of microcontrollers and modern CPU design. Created by John Walker, who started Autodesk/AutoCAD with others, it is a great read.

https://www.fourmilab.ch/hotbits/how3.html

I bought a number of items to test out the meter's functionality, which are discussed below.

It is recommended to read all of this document before commencing the project.

## Features:

Adafruit Feather Huzzah ESP8266 and ESP32 v2 support, OLED display, Li-Po battery.

Button inputs select between 3 OLED data display modes showing different formats, including bar meter strength graphs, detailed data, plus an auto-scaling histogram (photos below).

Radmon.org data upload every 60 seconds.

Web server diagnostics available via HTTP over port 80 (data, radmon.org upload status, debug, uptime).

Blue LED/Neopixel blue heartbeat, Red LED CPS (for Counts per Second <= 5).

Monitoring of MightyOhm's serial data for disconnections.

MightyOhm serial data over HTTP and Telnet.

mDNS multicast name registration.

Neopixel radmon status feedback.

OTA Software upgrades via WiFi.

NTP client for OLED and web server time display.

Unix syslog status messages to your syslog server.

Random number generator at a rate of 1 character per two pulses detected.

Customisable Bootup animation logo.

MQTT support on version 4 for the ESP32 platform.

## Parts list:

MightyOhm Geiger Counter DIY kit with GM tube and clear case (https://www.adafruit.com/products/483 and https://mightyohm.com/blog/products/geiger-counter/).

Either Feather Huzzah ESP8266 (https://www.adafruit.com/product/2821) or Feather Huzzah ESP32 v2 (https://www.adafruit.com/product/5400), ( https://core-electronics.com.au/adafruit-esp32-feather-v2-8mb-flash-2-mb-psram-stemma-qt.html )

Featherwing 128x64 OLED (https://www.adafruit.com/product/4650), ( https://core-electronics.com.au/adafruit-featherwing-oled-128x64-oled-add-on-for-feather-stemma-qt-qwiic.html ).

Stacking Headers for Feather - 12-pin and 16-pin female headers (https://www.adafruit.com/product/2830).

Li-Po battery to replace the original AAA batteries (https://www.adafruit.com/product/258).

Micro USB (Huzzah ESP8266) or USB C (Huzzah ESP32 v2) cable to connect your PC to the Feather.

Double-sided tape.

Female breadboard jumper wires to connect to Raspberry Pi if desired.

A test source of radiation, discussed in Testing below.

## Assembly

While assembling your MightyOhm, if you are yet to do so, do not install the AAA battery holder as this is where the Feather Huzzah is to be placed. If you have assembled your kit, you will need to remove the battery holder.

Start by soldering the stacking headers to the Huzzah and the supplied headers on the Featherwing OLED, as documented in Adafruit's online guide for your hardware. Familiarize yourself with all the documentation.

We first need to connect the MightyOhm Geiger to the Huzzah. You can use a breadboard to test your setup if desired, or you can simply dive in and solder the required 3 wires between the Huzzah and the MightyOhm. Optionally, you can include a 4th wire if you would like to enable and play with the random number generator. I used a small gauge wire so I was able to feed the 4 wires through the circuit board hole under the battery cover.

With the rear of the MightyOhm accessible, the first wire to connect is the MightyOhm Geiger serial TX pin, which is found on J7 pin 4 on the MightyOhm. This is connected to GPIO 13 on the Huzzah ESP8266 and GPIO 27 on the Huzzah ESP32 via the bottom of the board. This is pin 6 on the top, from left to right (from the front!) on both Huzzah versions and is the blue wire shown below.

If you want to include the random number functionality, wire from the MightyOhm center pulse pin on J6 to GPIO 12. GPIO 12 is pin 5 next to pin 6 noted above on both Huzzah versions and is the yellow wire in the image below.

Finally, connect the Huzzah's 3.3v and GND pins to the MightyOhm's battery connections, which are the red and black wires below. Refer to the images at the end of this document for both the Feather Huzzah's and the MightyOhm Geiger's header pinouts.

![WhyzaGC-underside wiring](https://user-images.githubusercontent.com/109115488/193185576-39bdc3c0-ae11-4c34-9da6-50260546a1f1.jpg)

You will notice in the above photo I have soldered a single stacking header pin on the bottom of the Huzzah on the VBUS/USB pin. I have soldered it at a right angle so I can connect Raspberry Pi's 5v supply to power the Feather Huzzah (and charge the Li-Po battery) via a jumper cable from the Pi's header.

Note that when the Huzzah is mounted in the MightyOhm, access is lost to the onboard USB serial connection, so you will need to upload the software mentioned below before final assembly. Once the software is loaded, you can use the OTA WiFi update feature if required when the Huzzah is mounted in its final position in the MightyOhm Geiger for upgrades.

After verifying the software below and the hardware is functional by connecting the Li-Po battery and checking operation, only then apply the double-sided tape and secure the Feather Huzzah to the MightyOhm AAA battery space and the Li-Po battery to the rear piece of the plastic case on the MightyOhm.

## Software:

Tested on Arduino IDE 1.8.19/2.1.0 and ESP8266 Boards 3.0.2/ESP32 2.0.5 with Arduino on Ubuntu Linux desktop 20.04/22.04 using both the Feather Huzzah ESP8266 and Feather Huzzah ESP32 v2.

To support the Feather Huzzah, add the following to File -> Preferences -> Additional Board Manager URL if not done already. This example has 2 entries for both the ESP8266 and ESP32 boards:

http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Then, in the Board Manager, install esp8266 or esp32 to reflect the hardware you have.

Finally, select the appropriate board via Tools -> Board -> ESP32 Arduino -> Adafruit Feather ESP32 V2 or Tools -> Board -> ESP8266 Boards -> Adafruit Feather Huzzah ESP8266.

Connect to your PC via the USB cable, and you should see the Huzzah fire up and show serial output @ 115200 baud in the Arduino IDE. After selecting the correct baud, you should be able to load and run sample code under File -> Examples to verify hardware functionality, such as Basics -> Blink or WiFi -> WiFiScan.

WhyzaGC Arduino C++ software for ESP8266 and ESP32 versions is available at:

Latest version available at https://github.com/Whyzanet/WhyzaGC

Download the correct C++ ino sketch file for your hardware, either ESP8266 or ESP32.

Note: The Feather Huzzah ESP8266 will require the  Frequency set to 160MHz to accommodate high values of CPS/CPM.

At the top of the sketch are the variables that will need changing for your specific environment. Register with radmon.org first so you have your required credentials.

The following libraries are available via the Arduino Library Manager and will need to be installed. I have listed the version that I tested under.

ESPSoftwareSerial(8.03)

ArduinoHttpClient(0.4.0)

Tlog(1.03) , Logs output to serial, telnet and http simultaneously

NTPClient(3.2.1)

Adafruit_GFX(1.11.5)

Adafruit_SH110X(2.1.8)

Adafruit Feather OLED(2.0.2)

Adafruit NeoPixel (ESP32)(1.11.0)

Uptime Library(1.0.0)

CircularBuffer(1.3.3)

PubSubClient(2.8)

The following libraries will need to be downloaded to your library directory:

movingAvg(2.3.1): [https://github.com/JChristensen/movingAvg]

Syslog(2.0.0): [https://github.com/arcao/Syslog]

ESP8266mDNS(1.2) (for ESP8266): [https://github.com/LaborEtArs/ESP8266mDNS]

The code in essence reads the serial data from the MightyOhm using SoftwareSerial via a GPIO pin, displays this data in various ways on the OLED, and uses movingAvg to average the CPM (Count per Minute) over 1 minute and subsequently upload the result to radmon.org via an HTTP client GET.

The histogram graph utilizes the circularBuffer library, which made the graphing relatively easy.

There are a number of networking functions which should be self-explanatory (mDNS, Web server, NTP client, serial to telnet/HTTP, Unix syslog, OTA upgrades).

The random number generator uses the pulse signal from the MightyOhm to generate an interrupt which will read the current value from a looping counter (basically a roulette wheel) that loops and counts from 0 to 15, which is then output as a hex value to serial/telnet/HTTP (Tlog). Collating and analyzing this random output is discussed below.

The code is broken up into individual functions, so if you want to see how one of the functions work, it should be straightforward.

Below is a list of the main functions:

wifion: Activate WiFi, setup Web server, OTA upgrades, and NTP client.

wifioff: Deactivate WiFi and radmon updates.

serialmon: Monitor the MightyOhm serial connection for disconnects longer than 15 seconds.

grab: Read the MightyOhm serial data via SoftwareSerial GPIO RX pin.

process: Check for valid MightyOhm EOL serial data, flash the heartbeat LED, extract the required data for variables, averages, and histogram display.

grabgraphdata: Grab current data for histogram circularBuffer.

averagedata: Average CPM data for radmon update.

mqttpub: Publish MQTT sensor data together with WiFi online and last radmon upload status. There are separate timers for each if you want to customize. Image below of HomeAssistant intergration via mosquitto MQTT broker. Pinned to CPU0 and only on the ESP32.

radmon: Once per minute, flash the NeoPixel purple (ESP32), perform the HTTP Get with radmon.org, and flash the NeoPixel (ESP32) red or green based on the result. Radmon will reject uploads if attempting to update before 30 seconds has expired since the last update. "Too soon" will appear in the log output ( TLOG and syslog ) if this occurs. 

handleRootPath: Configure the web diagnostics & upgrades with the HTTP server.

logstats: Send Unix syslog messages each hour.

buttons: Check for button input.

smallData: Display all data available in a small size font.

mediumData: Display all Geiger Counter Data in a medium-size font.

largeData: Display minimal CPS/CPM data in a large font size.

histogramData: Display histogram graph.

runntp: Update NTP time at the update interval.

randomise: Looping counter from 0 to 15 roulette wheel used for random number generation. Pinned to CPU0 on ESP32.

grabrandomnum: The function called by the interrupt to grab a number from the roulette wheel loop. Triggered by the pulse pin on the MightyOhm.

echorandom: Display the random number on a new line via Tlog to serial/telnet/HTTP.

setup: Initialization, animate bitmap logo, start WiFi if enabled.

bootupbitmap: Function holding bitmap data. Replace with your own logo via http://javl.github.io/image2cpp/ Ensure your logo is 128 x 64 pixels.

Upload the sketch and check for any missing libraries you may need.

Once running, input is via the 3 OLED Featherwing buttons. Button A and Button C will cycle up and down, respectively, through the 3 available data display modes. Button B selects the histogram display mode. See photos below of different display outputs.

Buttons B & C together will toggle WiFi disable and enable, which I found easy to select as the buttons are small enough and so close together. WiFi disable will also disable radmon.org updates and will require a reset to enable radmon.org uploads if radmon startup behavior is enabled. As such, if you toggle WiFi off and back on, you will have WiFi connectivity, but radmon updates are disabled until the next reset... useful for testing, as per below.

The USB serial port is set to 115200 baud and will display the MightyOhm Geiger serial output (when not mounted in the kit and accessible).

HTTP diagnostic data is available on port 80 via URL http://whyzagc-esp.local

Monitor easily via the following Linux terminal command:

watch -n 10 curl -s whyzagc-esp.local

To monitor the MightyOhm serial data remotely over HTTP, use http://whyzagc-esp.local:81, and serial data is also available in your favorite telnet program: telnet whyzagc-esp.local

Blue Heartbeat LED/Neopixel is triggered by an EOL from the Geiger serial output. It should flash 1 per second as a result. If Geiger serial EOL data is not detected, the heartbeat will stop.

Feather Huzzah ESP32 v2 Neopixel will flash purple every 60 secs when starting the radmon.org update and then will flash green when having completed a successful update, or red when detecting an unsuccessful update.

OTA upgrading of compiled sketch .bin via WiFi using the following URL: http://whyzagc-esp.local/upload using WebServer.h. 

To locate the correct binary for upload, within the Arduino IDE interface, select Sketch -> Export Compiled Binary, which will drop the compiled binary into the same location as the ino sketch file.

Alternately, version 4 software has  ArduinoOTA.h WiFi upgrade support and as such should show up as a network port in the Arduino IDE via mDNS where you can simply use Sketch --> Upload. Select Tools --> Ports -> WHyzaGC at a.b.c.d

WiFi (OTA) software updates, HTTP server diagnostics, and MightyOhm serial port redirection over telnet/HTTP all work around limitations with the USB serial port being inaccessible when the Huzzah is mounted in the MightyOhm's original AA battery location, preventing USB serial port access to upgrades, diagnostics, and MightyOhm raw data.

MQTT support on version 4 publishes the following subscriptions to the MQTT-configured broker which HomeAssistant digests

WhyzaGC/CPM: Average of count per minute published every 60 secs.

WhyzaGC/sieverts: Measured sieverts/hr from the MightyOhm output.

WhyzaGC/status: Publishes a status of online when WiFi is brought up successfully.

WhyzaGC/radmon_status: Publishes a status when radmon.org accessibility goes down or up."

Check out the Home Assistant Image below.

## Testing

There are several items you can collect to test radiation detection.

The SBM20 tube detects limited-range high-energy beta particle radiation (also known as electrons) and penetrating long-range gamma electromagnetic radiation (also known as photons), but not short-range alpha particle radiation, which is stopped by the tube's metallic outer composition. This is fine, as you don’t want to be playing with hazardous alpha radiation sources anyway (e.g., a smoke alarm’s Americium-241).

Vaseline glass contains uranium, making it a useful item for background level radiation testing and is easily available on eBay. The radiation from Vaseline glass is mostly stopped by aluminum, which fits the definition of being mostly beta radiation. Additionally, Vaseline glass/uranium glows a fluorescent bright green under UV light.

Other testing options include volcanic rock, granite benchtops, some types of Fiesta tableware (due to the colored glazing used), or chunks of uranium from http://unitednuclear.com/.

I bought a ‘nano-technology, ionizing anti EMF’ therapeutic volcanic pendant necklace, and while it gives a relatively weak count among my samples, only 90+ CPM, that works out to about 4.5 mSieverts/year, which is 4.5 times the annual normal background exposure of 1 mSieverts/year here in Australia (which is still very small). Its glossy product documentation has a long list of varied health benefits, including fighting cancer cells and protection from harmful 5G technology, if worn on the skin due to its ‘scalar energy’.

I also picked up from United Nuclear a small specimen bottle of uranium ore chunks, a Cloud Chamber Source (a bigger chunk of uranium ore), a geiger counter test card, and a Fiestaware glazed plate. The respective measured CPM readings in order of radioactivity are below:

Background radiation: 18 CPM

Therapeutic volcanic pendant: 90 CPM

Vaseline glass: 173 CPM

United Nuclear small specimen bottle of uranium ore chunks: 1700 CPM

United Nuclear geiger counter test card: 2700 CPM

No name tableware red/orange glazed plate: 6500 CPM

Fiesta tableware red/orange glazed plate: 9200 CPM

United Nuclear Cloud Chamber Source (chunk of uranium): 27300 CPM

The maximum tested using a combination of samples listed above and both the SBM-20 tube and an SBT-11 pancake detector: 1366 CPS or 82000 CPM.

An airplane flight would also be an interesting test environment due to increased cosmic radiation levels at altitude. Latitude also affects background cosmic radiation, with higher levels the closer you are to the poles. An interesting calculator is available at https://jag.cami.jccbi.gov/

Fiesta tableware plate, therapeutic volcanic pendant, United Nuclear uranium ore chunks, and geiger counter test card.

![Fiestaware-Testcard-Uorejpg](https://user-images.githubusercontent.com/109115488/189282549-54f30927-5ccf-40b2-9b11-2017fbfefbd7.jpg)

## ESP8266/ESP32/OLED specific settings

As noted above, the Feather Huzzah ESP8266 will require the CPU Frequency set to 160MHz to accommodate high values of CPS/CPM.

Other than the expected differences in libraries for the ESP8266 & ESP32, respectively (ESP8266WiFi.h/WiFi.h, ESP8266WebServer.h/WebServer.h, and ESP8266mDNS/ESPmDNS), the 2nd LED differences (Blue LED vs. Neopixel RGB LED), and pinout changes, the Feather Huzzah ESP32 CPU is also dual-core.

As such, in the ESP32 code, I have set both the radmon.org update, MQTT publish and randomise functions to utilize CPU0 rather than CPU1, which all other code runs on by default.

This solves two issues on the ESP8266 platform due to its single CPU and my less than proficient programming skills. Firstly, while performing the TCP connection setup and GET request for the radmon.org update, the (single) CPU is unavailable for other tasks. Since the typical TCP setup, data exchange, and teardown takes at least 1.5+ seconds in my environment (and up to N secs!), resulting in the ESP8266 missing N lines of serial data from the MightyOhm, one per second. This is not a significant issue as the code is reading the MightyOhm CPM (Counts per Minute) value, which is itself averaged and uploading the resulting Arduino-calculated 1-minute rolling average. So the impact of missing a couple of average values is negligible overall. It explains why the code is measuring the upload time and showing it in the web diagnostics. I did try schedulers and yield() without success on the ESP8266....

Secondly, because the roulette wheel function generating the random data is time-critical, any sharing of CPU will corrupt the random data. My initial tests confirm this.

This is not a problem on the ESP32, where the radmon.org function (and the functions it calls) and the randomise function are both pinned to CPU0 while the default CPU1 is free to carry out other tasks. When not uploading to radmon, CPU0 is available to the randomise function.

The other hardware consideration is that I can confirm that OLED screen burn-in does occur with the default contrast and use over 1000 hours, as noted on the Adafruit site, resulting in a contrast deviation as the datasheet explains it. As such, I have now set the contrast to a minimum to preserve the screen. This setting is near the top of the ino file if you desire to change it.

## Random number generator.

It is worth mentioning this link again if you are interested in how random numbers can be generated from natural events such as radioactive decay:

https://www.fourmilab.ch/hotbits/how3.html

I was also inspired by this project, which has some good ideas and explained the tools for use in validating random numbers:

https://github.com/gbonacini/nuclear_random_number_generator

As mentioned, I added a simple random number generator, which seems to work well on the ESP32 platform. The random number generator uses the pulse signal from the MightyOhm to generate a hardware interrupt, which will read the current value from a looping counter (basically a roulette wheel) that counts from 0 to 15. This value is then output as a hex value to serial/telnet/HTTP (Tlog).

It is disabled by default. You can enable it via 'true' or 'false' at the top section of the ino file:

bool randomon = true; // enable random number generator

Generating data using only background radiation levels takes some time as you only generate one random character per 2 pulses. I did testing with high counts utilizing some of the radiation samples I have and found that the output data was far from random. I believe this is due to the SMB20 saturating and the associated dead zone when the tube is rendered insensitive.

The random numbers are available via the serial, HTTP, or telnet connection. A character is output on a new line for each pulse when enabled.

To analyze the data, I installed 'ent' (written by John Walker mentioned above from http://www.fourmilab.ch/) via:

apt-get install ent

I then downloaded the test directory from the nuclear_random_number_generator project above, 'cd' to this test directory where you can run:

./test_random_numbers.sh

to format and test the included rnd_nums.txt data.

Running

ent -c rnd_nums.bin

will analyze the data per character, while

ent -b -c rnd_nums.bin

will analyze the data per bit.

The manpage for 'ent' describes the tests performed.

I collected random data from the Huzzah via the telnet data output using the following command on my Linux desktop:

nc whyzagc-esp.local 23 | tee -a rnd_nums.txt

and waited about 5 days until I had about 15M raw data, which gave me around 50K of binary random data once filtered.

I had to change line 6 in test_random_numbers.sh from:

cat rnd_nums.txt | grep -v '^CPM' | tr -d '\r\n' | head -204800 > rnd_nums.clean.txt

to the rather convoluted:

cat rnd_nums.txt | cut -c -2 | grep --binary-files=text -v 'CP|Te|19|C|BP|PS|GP' | cut -c -1 | grep --binary-files=text '0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f' | grep --binary-files=text -v -e '^$' | tr -d '\r\n' > rnd_nums.clean.txt

to ensure the filtered data collected via the nc command was free of the MightyOhm serial output and nc's output messages. You may need to modify this if your environment is different from mine (e.g., the '19' in the search field is a match from the first two characters of 192.168.0.100, the address nc is connecting to). This address is obviously not meant to be in the final random data.

Once you have run test_random_numbers.sh over your new data in rnd_nums.txt, you can analyze your data as per above with the ent command or visually with:

./scatter.py rnd_nums.bin

I have attached my ESP32 scatter graph below.

I have initially found that the ESP8266 platform does not pass the Chi-square distribution test under ent and is visibly not random when inspecting the scatter graph. This is no doubt due to the limitations of only one CPU where you cannot dedicate the time-critical randomize counter to a spare CPU. On the ESP32 hardware, I have pinned the randomize function to the previously underutilized CPU0. CPU0 was only performing the once per minute Radmon & MQTT uploads, so other than these uploads, which will pause the randomize function, it is available for dedicated use.

So with the ESP32, a $40 micro marvel of today's world, it is reassuring to be able to actually confirm that the universe really IS composed of such truly random events or occurrences of quantum tunneling, which can't be explained by our classical understanding of physics, despite even Einstein believing, 'God does not play dice with the universe.' If Einstein could ask today's AI, 'What are the implications and observations that can be made when understanding the universe is based on random quantum events?' he would see a list of several fascinating observations and interpretations that we can now make since Einstein's ( 1879 - 1955 ) time. Look it up yourself for a good read.

## Additional Connections:

I am connecting the VBUS/USB pin on the Huzzah to a Raspberry Pi 5V pin via the Pi GPIO header, as well as a ground connection between the two boards. The current is usually around 110 mA, though it will peak at around 300 mA when charging the Li-Po battery, which probably means any 5V supply will suffice.

I am also connecting the Geiger serial TX pin (J7 pin 4) to the Raspberry Pi 4's additional serial Rx pins on UART 4 and 5 (the default serial port is already in use) by setting dtoverlay=uart4 and dtoverlay=uart5 in the Pi's /boot/config.txt file and then connecting the Geiger serial TX pin to both pins 33 and 21. These additional serial ports are found under /dev/ttyAMA1 and /dev/ttyAMA2 under Raspberry Pi OS Bullseye. On one port, I have configured a bash script to perform radmon.org updates for use if I don't want to upload from the Feather Huzzah for whatever reason. The other port is used by a Perl script that displays graphs using rrdtool. See the RPI 4 pinout below for the additional serial ports or use "raspi-gpio funcs" from the command line to show GPIO additional functions.

See this link for the Perl script. Read the whole page before starting:

https://mightyohm.com/forum/viewtopic.php?t=3504

Finally, I also have the MightyOhm pulse pin (J6 pin 2) connected to pin 35 on the Pi 4, so that a bash script can look for a rising edge on GPIO 19. I use this to log occurrences of high CPS (Counts per Second) for when I need to reconfirm the randomness of the universe.

## Images

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

Scatter Graph visually showing the true randomness of the detected events on the ESP32 platform ( around 100K bytes of collected random data ). A non random result would show patterns of waves and/or clumps and empty spaces, which are not evident in the true random image below.

![ScatterGraph-ESP32-gieger-randomiser](https://github.com/Whyzanet/WhyzaGC/assets/109115488/eecac13f-458c-4d39-99c5-5dce6f03b634)

HomeAssistant intergration via Mosquito MQTT broker.

![Screenshot from 2023-08-07 20-22-36](https://github.com/Whyzanet/WhyzaGC/assets/109115488/f6982bd9-1e5f-4ed0-b0ef-1f354ab21123)

Raspberry Pi Graphs using rrdtool

![rpi-graph](https://user-images.githubusercontent.com/109115488/184986901-79bdcfbd-5aa8-444f-a940-c020fcd8be97.png)

Radmon Site

![Radmon-site](https://user-images.githubusercontent.com/109115488/184986769-9446e31d-5155-4078-8dea-657e5d715cdd.png)
 
Radmon data

![radmon-12hr](https://user-images.githubusercontent.com/109115488/184986777-30e85e87-a652-45a8-9987-b25e3880f064.png)

Adafruit Feather Huzzah ESP8266 pinout 

![FeatherHuzzahESP8266pinout](https://user-images.githubusercontent.com/109115488/183532848-67e19153-23b0-45d9-b821-0f4a29993b36.png)

Adafruit Feather Huzzah ESP32 v2 pinout. ADC2 is used by WiFi if enabled and not available for IO. Pin 32 is also not available as it is used by the B button on the OLED screen feather.

![FeatherHuzzahESP32pinout](https://user-images.githubusercontent.com/109115488/183532850-095e769c-4976-42ac-ab86-83e7bfbfaed9.png)

MightyOhm Geiger schematic showing header pinout configurations.

![geiger_sch_fixedR5R6](https://user-images.githubusercontent.com/109115488/198948137-f1748bfc-4e6d-4b7e-b820-efd574d00066.png)

Raspberry Pi 4 pinout showing additional serial ports. Use "raspi-gpio funcs" to list alternative GPIO functions such as serial ports. This image is missing TXD3 which "raspi-gpio funcs" shows is GPIO 4 or pin 7. Further info here. https://raspberrypi.stackexchange.com/questions/104464/where-are-the-uarts-on-the-raspberry-pi-4

![Raspberry-Pi-pinout](https://github.com/Whyzanet/WhyzaGC/assets/109115488/3055c326-28ef-4a2f-9da5-8388532675fe)

## Latest Version

The latest version of this document and project is available via:

https://github.com/Whyzanet/WhyzaGC
