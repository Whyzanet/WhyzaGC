const char* release = "WhyzaGC v3.0";
const char* releasedate = "Released 17/9/2022";
bool WIFIEnable = true; // set startup wifi client behaviour
const char* ssid = "CHANGEME"; // Set wifi SSID
const char* password = "CHANGEME"; // Set wifi password
int TZ = +10; // Set local timezone offset from UTC CHANGEME
bool radmonen = true; // Set startup radmon.org behaviour. Will enable WIFI if reqd
const char* radmonuser = "CHANGEME";  // Set radmon.org username
const char* radmonpass = "CHANGEME"; // Set radmon.org password
const char* ntpServer = "pool.ntp.org"; // Set ntp server
String hostname = "WhyzaGC-ESP"; // Set wifi hostname
uint8_t mode = 5;  // Startup default OLED display mode - Graph Display ( Buttons A&B )
#include <SoftwareSerial.h>
SoftwareSerial mySerial(27, 33); // Gieger serial port directly connected on RX, TX ( no TX connection needed as only receiving data )
#include <Syslog.h>
#define SYSLOG_SERVER "192.168.0.138" // Set syslog server
#define SYSLOG_PORT 514 // Syslog port
#define DEVICE_HOSTNAME "WhyzaGC-ESP" // Set syslog hostname
const char* APP_NAME = "my-function";
#include <WebServer.h>
WebServer server(80);  // Set web server port
int contrast = 0; // reduce OLED contrast to preserve OLED lifespan

#include <TLog.h>      // The T-Logging library.
// Run a telnet service on the default port (23) which shows what is
// sent to Serial if you telnet to it.
//
#include <TelnetSerialStream.h>
TelnetSerialStream telnetSerialStream = TelnetSerialStream();
// Likewise let http://<ipaddres>:81/ show the log in a webbrowser.
//
#include <WebSerialStream.h>
WebSerialStream webSerialStream = WebSerialStream(81);

#include <ESPmDNS.h>
#include <Update.h>
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
#include <ArduinoHttpClient.h>
#include <movingAvg.h>  // we need to do some averaging
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_FeatherOLED_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include "uptime_formatter.h"
#include <CircularBuffer.h>  // Histogram data 
#define buttonA 15
#define buttonB 32
#define buttonC 14
Adafruit_FeatherOLED_SH110X oled = Adafruit_FeatherOLED_SH110X();
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer);
WiFiUDP udpClient;
movingAvg cpmminavg(60);// keep a 1 minute moving average of CPM data to upload
movingAvg cpmhouravg(3600);
movingAvg timerdelayavg(60);
movingAvg timerdelaydayavg(1440);
CircularBuffer<int, 115> xpoints;  // mode 5 histogram graph data
unsigned long uploadcpm = 0;
unsigned long cpmhour = 0;
unsigned long uploaddelay = 0;  // measure the radmon upload time.
unsigned long lastupdate = 0;
unsigned long avgdelay = 0;
unsigned long logdelay = 0;
unsigned long timeravg = 0;
unsigned long timerdayavg = 0;
unsigned long buttontimer = 0;
String Release = release;
String ReleaseDate = releasedate;
String Hostname = hostname;
int statusCode = 0;
uint8_t oldmode;
bool geigerConnected = false;
String geigerconnected;
unsigned long geigerTimeout = 0;
bool gotButton = false;
String geigerSerial;
unsigned char geigerdata;
String webdata;
int barz = 0;
int cps = 0;
int cpm = 0;
int xsize = 0;
int xplot = 0;
float sieverts = 0;
float svyear = 0;
float battery = oled.getBatteryVoltage();
char buffer[255];
unsigned long timer = 0;
unsigned long timertotal = 0;
float scale = 0; // Setup histogram variables
int recenthi = 0;
int previoushi = 0;
bool scale1 = false;
bool scale2 = false;
bool scale3 = false;
bool scale4 = false;
bool scale5 = false;
bool scale6 = false;
bool scale7 = false;
bool scale8 = false;
bool scale9 = false;
bool scale10 = false;
bool scale11 = false;
unsigned long grabgraphdatadelay = 0;
#define PIN 0 //NeoPixel
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define COMMON_ANODE
TaskHandle_t Task1;

//***Functions***
void stopWiFiAndSleep() {  // Stop sleep
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(3);
}
void wifion () {  //enable WiFi
  sprintf(buffer, "%s\n%s\n", release, releasedate);
  Serial.println("");
  Serial.println(" ________  __           ____                         __");
  Serial.println("|  |  |  ||  |_ .--.--.|_  / .-----.   .-----.-----.|  |_");
  Serial.println("|  |  |  ||    ||  |  | / /_ | -   | _ |     |  -__||   _|");
  Serial.println("|________||__|_||__   ||____||___|_||_||__|__|_____||____|");
  Serial.println("                   |__|        ");

  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  Serial.print(buffer);
  oled.print(buffer);
  battery = oled.getBatteryVoltage();
  oled.setBattery(battery);
  oled.renderBattery();
  oled.println("\n");
  APP_NAME = "wifion: ";
  Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
  if (WIFIEnable == true) {
    sprintf(buffer, "Connecting to\n%s\n", ssid);
    oled.print(buffer);
    Serial.print(buffer);
    oled.display();
    WiFi.setHostname(Hostname.c_str());
    WiFi.begin(ssid, password);
    int retries = 0;
    while ((WiFi.status() != WL_CONNECTED) && (retries < 15)) {
      retries++;
      delay(500);
      Serial.print(".");
      oled.print(".");
      oled.display();
    }
  }
  sprintf(buffer, "\nWiFi status\nIPv4: %s\n", WiFi.localIP().toString().c_str());
  Serial.print(buffer);
  oled.print(buffer);
  oled.display();
  delay(1500);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(hostname);
    timeClient.begin(); // Start NTP client
    timeClient.setTimeOffset(3600 * TZ);
    timeClient.setUpdateInterval(43200000);
    server.on("/", handleRootPath);    // Setup web servers for root and upload
    server.on("/upload", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Stats available:\n");
    Serial.printf("http://%s.local\n", hostname);
    Serial.printf("Software upload:\n");
    Serial.printf("http://%s.local/upload\n", hostname);
  } else {
    Serial.println("WiFi Failed");
  }
  syslog.log(LOG_INFO, buffer);
  oled.clearDisplay();
  oled.setCursor(0, 0);
  runntp();
}
void wifioff () { // disable WiFi
  APP_NAME = "wifioff: ";
  Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
  syslog.log(LOG_INFO, "WiFi Disconnect and Powersave");
  syslog.log(LOG_INFO, "Will require a reboot to enable radmon.org if enabled");
  radmonen = false;
  delay(3);
  stopWiFiAndSleep();
  oled.clearDisplay();
  oled.setTextSize(1);
  battery = oled.getBatteryVoltage();
  oled.setBattery(battery);
  oled.renderBattery();
  oled.setCursor(0, 0);
  sprintf(buffer, "%s\n%s\n\nDisconecting from\n%s\n\nWiFi disconnected\n%s\n\nWifi powersaving on", release, releasedate, ssid, WiFi.localIP().toString().c_str());
  oled.print(buffer);
  Serial.print(buffer);
  oled.display();
  delay(2500);
  oled.clearDisplay();
}
void grabgraphdata () {  // grab histogram graph data for mode 5 display
  if (millis() > (grabgraphdatadelay + 1000)) {
    //  if (cpm >= 1) {
    xpoints.push(cpm);
    xsize = xpoints.size();
    xplot = xpoints.last();

    //  for (decltype(xpoints)::index_t i = 0; i < xpoints.size(); i++) {
    //        Serial.print(xpoints[i]);
    //  }
    //  Serial.println(" ");
    grabgraphdatadelay = millis();
  }
}
void logstats () {  // remote syslog status
  if (millis() > (logdelay + 3600000)) {
    APP_NAME = "logstats:";
    Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
    syslog.logf(LOG_INFO, "Latest 1 min average CPM: %d", uploadcpm);
    syslog.logf(LOG_INFO, "Latest 1 hour average CPM: %d", cpmhour);
    syslog.logf(LOG_INFO, "Latest HTTP response: %d", statusCode);
    logdelay = millis();
  }
}

void averagedata () { // calculate CPM data average for radmon upload
  if (millis() > (avgdelay + 550)) {
    uploadcpm = cpmminavg.reading(cpm);
    cpmhour = cpmhouravg.reading(cpm);
    avgdelay = millis();
  }
}
void radmon (void * ptParameters ) {  // perfrom the HTTP Get for radmon.org every minute
  for (;;) {
    if (radmonen == 1 && (geigerConnected == true)) {
      if (WIFIEnable == 0 || WiFi.status() != WL_CONNECTED) {
        statusCode = 0;
      }
      if (WIFIEnable == 1 && (millis() > (uploaddelay + 59999 - timertotal))) {
        if (WiFi.status() != WL_CONNECTED) {
          wifion();
        }
        else {
          pixels.clear();
          pixels.setPixelColor(0, pixels.Color(255, 0, 255));  // flash neopixel purple when starting http client upload
          pixels.show();
          delay(80);
          pixels.clear();
          pixels.show();
          delay(80);
          pixels.setPixelColor(0, pixels.Color(255, 0, 255));
          pixels.show();
          delay(80);
          pixels.clear();
          pixels.show();
          String ptr = "/radmon.php?function=submit";
          ptr += "&user=";
          ptr += radmonuser;
          ptr += "&password=";
          ptr += radmonpass;
          ptr += "&value=";
          ptr += uploadcpm;
          ptr += "&unit=CPM";
          timer = (millis());
          WiFiClient wifi;
          HttpClient client = HttpClient(wifi, "www.radmon.org", 80);
          client.get(ptr);
          statusCode = client.responseStatusCode();
          lastupdate = uploadcpm;
          uploaddelay = millis();
          timertotal = (millis() - timer);
          //    Serial.println(timertotal);
          timeravg = timerdelayavg.reading(timertotal);
          timerdayavg = timerdelaydayavg.reading(timertotal);
          if (statusCode == 200) {
            flashgreen ();  // successful update
          }
          else {
            flashalertred ();  // unsuccessful update
          }
          //    Serial.println(timeravg);
          //          Serial.print("radmon uploaded using core ");
          //          Serial.println(xPortGetCoreID());
        }
      }
    }
    delay(1000);
  }
}
void runntp () {  // run  the NTP client
  timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
  //Serial.println(timeClient.getFormattedDate());
}
void flashgreen () { // radmon update successful
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
  delay(80);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
  delay(80);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
}
void flashalertred () { // radmon update failed
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
  delay(80);
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
  delay(80);
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
}
void flashblue () { // flash blue NeoPixel HeartBeat for every serial EOL received ( should be 1 sec )
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  delay(80);
  pixels.clear();
  pixels.show();
}
void flashred () {  // flash red LED CPS times
  for (int i = 1; i <= cps; i++) {
    digitalWrite(13, HIGH); // Flash red LED * cps
    delay(40);
    digitalWrite(13, LOW);
    delay(40);
  }
}
void handleRootPath() {  // Web diagnostics page
  server.send(200, "text/plain", Release + ("\n") + (
                ReleaseDate) + ("\n") + (
                "Hardware: Feather Huzzah ESP32 v2") + ("\n") + ("\n") + (
                " ________  __           ____                         __") + ("\n") + (
                "|  |  |  ||  |_ .--.--.|_  / .-----.   .-----.-----.|  |_") + ("\n") + (
                "|  |  |  ||    ||  |  | / /_ | -   | _ |     |  -__||   _|") + ("\n") + (
                "|________||__|_||__   ||____||___|_||_||__|__|_____||____|") + ("\n") + (
                "                   |__|        ") + ("\n") + ("\n") + (
                timeClient.getFormattedDate()) + ("\n") + (
                "Uptime: ") + (uptime_formatter::getUptime()) + ("\n") + ("\n") + (
                "Geiger Connected: ") + geigerconnected + (" ") + ("\n") + ("\n") + (
                "Rolling Average CPM: ") + uploadcpm + ("\n") + (
                "Rolling 1 hour Average CPM: ") + cpmhour + ("\n") + ("\n") + (
                "uSieverts/Hour: ") + sieverts + ("\n") + (
                "mSieverts/Year: ") + svyear + ("\n") + ("\n") + (
                "Last Radmon Update(secs): ") + ((millis() - uploaddelay) / 1000) + ("\n") + (
                "Radmon HTTP Get status code: ") + statusCode + ("\n") + (
                "Last Upload Delay(ms): ") + timertotal + ("\n") + (
                "Rolling 1 hour Average Upload Delay(ms): ") + timeravg + ("\n") + (
                "Rolling 1 day Average Upload Delay(ms): ") + timerdayavg + ("\n") + (
                "Last Radmon Update CPM: ") + lastupdate + ("\n\n") + (
                "Current Data:") + webdata + ("\n\n") + (
                "Debug:\n") + (
                "Scale: ") + scale + (" recenthi: ") + recenthi + (" xplot: ")  + xplot + ("\n")
             );
}

void buttons () {  // check for button presses
  if (millis() > (buttontimer + 500)) {
    buttontimer = millis();
    if (!digitalRead(buttonC) && (!digitalRead(buttonB))) { // Check if a button is pressed and change mode accordingly
      oldmode = mode;
      WIFIEnable = !WIFIEnable;
      mode = 5;
      gotButton = true;
    }
    else if (!digitalRead(buttonA)) {
      mode = (mode + 1);
      if (mode == 5)
        mode = 1;
      gotButton = true;
    }
    else if (!digitalRead(buttonB)) {
      mode = 4;
      gotButton = true;
    }
    else if (!digitalRead(buttonC)) {
      mode = (mode - 1);
      if (mode == 0 ) {
        mode = 4;
      }
      gotButton = true;
    }

    if (gotButton)
      delay(25); // Debounce
    switch (mode) {   // Display current mode
      case 1: // All data and graph ( max 110 cpm )
        smallData();
        break;
      case 2: // Geiger data and graph ( max 110 cpm )
        mediumData();
        break;
      case 3: // Large count stats and background graph ( max 28 cpm )
        largeData();
        break;
      case 5: //Toggle WIFI on/off - Buttons B&C
        toggleWiFi();
        break;
      case 4: //Graph Data, autoscale max 16, 32, 64, 128, 256
        histogramData();
        break;
    }
  }
}
void smallData () { // All data and graph ( max 110 cpm )
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.print("HTTP Get:");
  if (statusCode >= 999 )
    statusCode = 0;
  oled.print(statusCode);
  oled.setCursor(80, 0);
  oled.println(timeClient.getFormattedTime());
  oled.print("WiFi:");
  if (WiFi.status() != WL_CONNECTED)
    oled.println("N/A");
  else
    oled.println(WiFi.localIP());
  oled.setTextSize(2);
  oled.print("CPS:");
  oled.print(cps);
  oled.setTextSize(1);
  if ( cpm < 1000) {
    oled.setCursor(67, 16);
  }
  else {
    oled.setCursor(79, 16);
  }
  oled.print(" A:");
  oled.print(uploadcpm);
  oled.setCursor(127, 0);
  oled.setTextSize(2);
  oled.println();
  oled.println();
  oled.print("CPM:");
  if (cpm >= 1) {
    oled.println(cpm);
  }
  oled.setTextSize(1);
  oled.print("uSieverts/Hour:");
  oled.println(sieverts);
  oled.print("mSieverts/Year:");
  oled.println(svyear);
  if ( cpm < 1000) {
    if (cpm >= 110) {
      barz = 7;
    } else if (cpm < 110 & cpm >= 35) {
      barz = 6;
    } else if (cpm < 35 & cpm >= 28) {
      barz = 5;
    } else if (cpm < 28 & cpm >= 23) {
      barz = 4;
    } else if (cpm < 23 & cpm >= 18) {
      barz = 3;
    } else if (cpm < 18 & cpm >= 13) {
      barz = 2;
    } else if (cpm < 13 & cpm >= 9) {
      barz = 1;
    } else {
      barz = 0;
    }
    for (int b = 0; b <= barz; b++) {
      oled.fillRect(90 + (b * 5), 43 - (b * 5), 3, b * 5, WHITE);
    }
  }
  oled.display();
}
void mediumData () { // Geiger data and graph ( max 110 cpm )
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.print("uSv/hr:");
  oled.print(sieverts);
  oled.setCursor(72, 0);
  oled.print("Avg:");
  oled.println(uploadcpm);
  oled.setCursor(0, 16);
  oled.setTextSize(1);
  oled.print("CPS: ");
  oled.setCursor(30, 8);
  oled.setTextSize(3);
  oled.println(cps);
  oled.setTextSize(1);
  oled.print("mSv/yr:");
  oled.println(svyear);
  oled.setTextSize(3);
  if (cpm >= 1) {
    oled.setCursor(0, 48);
    oled.setTextSize(1);
    oled.print("CPM: ");
  }
  oled.setCursor(30, 40);
  oled.setTextSize(3);
  oled.println(cpm);
  if (cpm >= 110) {
    barz = 7;
  } else if (cpm < 110 & cpm >= 35) {
    barz = 6;
  } else if (cpm < 35 & cpm >= 28) {
    barz = 5;
  } else if (cpm < 28 & cpm >= 23) {
    barz = 4;
  } else if (cpm < 23 & cpm >= 18) {
    barz = 3;
  } else if (cpm < 18 & cpm >= 13) {
    barz = 2;
  } else if (cpm < 13 & cpm >= 9) {
    barz = 1;
  } else {
    barz = 0;
  }
  for (int b = 0; b <= barz; b++) {
    oled.fillRect(90 + (b * 5), 35 - (b * 5), 3, b * 5, WHITE);
  }
  oled.display();
}
void largeData () { // Large count stats and background graph ( max 28 cpm )
  if ( cpm >= 1000) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextSize(4);
    if (cpm >= 1) {
      oled.print(cps);
    }
    oled.setTextSize(1);
    oled.setCursor(75, 0);
    oled.println(timeClient.getFormattedTime());
    oled.setCursor(75, 8);
    if (statusCode >= 999 )
      statusCode = 0;
    oled.print("U/L:");
    oled.println(statusCode);
    oled.setCursor(75, 16);
    oled.print("Avg:");
    oled.print(uploadcpm);
    oled.setCursor(127, 0);
    oled.setTextSize(4);
    oled.println();
    oled.println(cpm);
    oled.display();
  }
  else {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextSize(4);
    if (cpm >= 1) {
      oled.print(cpm);
    }
    oled.setTextSize(1);
    oled.setCursor(69, 0);
    oled.println(timeClient.getFormattedTime());
    oled.setCursor(69, 8);
    if (statusCode >= 999 )
      statusCode = 0;
    oled.println(statusCode);
    oled.setCursor(69, 16);
    oled.print(uploadcpm);
    oled.setCursor(127, 0);
    oled.setTextSize(4);
    oled.println();
    oled.println(cps);
    if (cpm >= 28) {
      barz = 19;
    } else if (cpm < 28 & cpm >= 27) {
      barz = 18;
    } else if (cpm < 27 & cpm >= 26) {
      barz = 17;
    } else if (cpm < 26 & cpm >= 24) {
      barz = 16;
    } else if (cpm < 24 & cpm >= 23) {
      barz = 15;
    } else if (cpm < 23 & cpm >= 22) {
      barz = 14;
    } else if (cpm < 22 & cpm >= 21) {
      barz = 13;
    } else if (cpm < 21 & cpm >= 20) {
      barz = 12;
    } else if (cpm < 20 & cpm >= 19) {
      barz = 11;
    } else if (cpm < 19 & cpm >= 18) {
      barz = 10;
    } else if (cpm < 18 & cpm >= 17) {
      barz = 9;
    } else if (cpm < 17 & cpm >= 16) {
      barz = 8;
    } else if (cpm < 16 & cpm >= 15) {
      barz = 7;
    } else if (cpm < 15 & cpm >= 14) {
      barz = 6;
    } else if (cpm < 14 & cpm >= 13) {
      barz = 5;
    } else if (cpm < 13 & cpm >= 12) {
      barz = 4;
    } else if (cpm < 12 & cpm >= 11) {
      barz = 3;
    } else if (cpm < 11 & cpm >= 10) {
      barz = 2;
    } else if (cpm < 10 & cpm >= 9) {
      barz = 1;
    } else {
      barz = 0;
    }
    for (int b = 0; b <= barz; b++) {
      oled.fillRect(27 + (b * 5), 60 - (b * 3), 3, b * 3, WHITE);
    }
    oled.display();
  }
}

void toggleWiFi () { //Toggle WIFI on/off
  if (WIFIEnable == 1 )
    wifion();
  if (WIFIEnable == 0 ) {
    wifioff();
    stopWiFiAndSleep();
  }
  oled.clearDisplay();
  mode = oldmode;
}
void histogramData () { //Graph Data, autoscale max 16, 32, 64, 128, 256
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(36, 0);
  //  oled.print("mSv/y: ");
  oled.println(svyear);
  oled.setCursor(80, 0);
  oled.println(timeClient.getFormattedTime());
  if (recenthi == 11) {
    scale = 0.00390625 ;
  } else if (recenthi == 10) {
    scale = 0.0078125 ;
  } else if (recenthi == 9) {
    scale = 0.015625 ;
  } else if (recenthi == 8) {
    scale = 0.03125 ;
  } else if (recenthi == 7) {
    scale = 0.0625 ;
  } else if (recenthi == 6) {
    scale = 0.125 ;
  } else if (recenthi == 5) {
    scale = 0.25 ;
  } else if (recenthi == 4) {
    scale = 0.5 ;
  } else if (recenthi == 3) {
    scale = 1 ;
  } else if (recenthi == 2) {
    scale = 2;
  } else if (recenthi == 1) {
    scale = 4;
  } else {
    scale = 2 ;
  }
  for (decltype(xpoints)::index_t i = 0; i <= xpoints.size(); i++) {
    oled.fillRect( i + 12 , 65 - (int(scale * xpoints[i])) , 1 , (int(scale * xpoints[i])) , WHITE);
    //    Serial.print(xpoints[i]);
    //    Serial.println(int(scale * xpoints[i]));
    for (decltype(xpoints)::index_t i = 0; i <= xpoints.size(); i++) {
      if (xpoints[i] >= 8193 ) {
        recenthi = 11;
        scale11 = true;
      }
      if ( xpoints[i] < 8193 && xpoints[i] >= 4097 && scale11 == false ) {
        recenthi = 10;
        scale10 = true;
      }
      if ( xpoints[i] < 4097 && xpoints[i] >= 2049 && scale11 == false && scale10 == false ) {
        recenthi = 9;
        scale9 = true;
      }
      if ( xpoints[i] < 2049 && xpoints[i] >= 1025 && scale11 == false && scale10 == false && scale9 == false ) {
        recenthi = 8;
        scale8 = true;
      }
      if ( xpoints[i] < 1025 && xpoints[i] >= 513 && scale11 == false && scale10 == false && scale9 == false  && scale8 == false ) {
        recenthi = 7;
        scale7 = true;
      }
      if ( xpoints[i] < 513 && xpoints[i] >= 257 && scale11 == false && scale10 == false && scale9 == false  && scale8 == false && scale7 == false ) {
        recenthi = 6;
        scale6 = true;
      }
      if ( xpoints[i] < 257 && xpoints[i] >= 129 && scale11 == false && scale10 == false && scale9 == false  && scale8 == false && scale7 == false && scale6 == false ) {
        recenthi = 5;
        scale5 = true;
      }
      if ( xpoints[i] < 129 && xpoints[i] >= 65 && scale11 == false && scale10 == false && scale9 == false  && scale8 == false && scale7 == false && scale6 == false && scale5 == false ) {
        recenthi = 4;
        scale4 = true;
      }
      if ( xpoints[i] < 65 && xpoints[i] >= 33 && scale11 == false && scale10 == false && scale9 == false && scale8 == false && scale7 == false && scale6 == false && scale5 == false && scale4 == false ) {
        recenthi = 3;
        scale3 = true;
      }
      if ( xpoints[i] < 33 && xpoints[i] >= 17 && scale11 == false && scale10 == false && scale9 == false && scale8 == false && scale7 == false && scale6 == false && scale5 == false && scale4 == false && scale3 == false ) {
        recenthi = 2;
        scale2 = true;
      }
      if ( xpoints[i] < 17 && xpoints[i] >= 1 && scale11 == false && scale10 == false && scale9 == false && scale8 == false && scale7 == false && scale6 == false && scale5 == false && scale4 == false && scale3 == false && scale2 == false ) {
        recenthi = 1;
        scale1 = true;
      }
    }
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print(int(64 / scale));
    oled.setCursor(0, 14);
    oled.print(int(48 / scale));
    oled.setCursor(0, 28);
    oled.print(int(32 / scale));
    oled.setCursor(0, 42);
    oled.print(int(16 / scale));
    oled.setCursor(0, 57);
    oled.print("0");
    oled.setTextColor(BLACK);
    oled.setCursor(14, 48);
    oled.setTextSize(2);
    oled.print(cps);
    if ( cps < 100 ) {
      oled.setTextSize(1);
      oled.print("CPS");
    }
    oled.setCursor(58, 48);
    oled.setTextSize(2);
    if (xplot >= 1); {
      oled.print(xplot);
    }
    oled.setTextSize(1);
    oled.print("CPM");
    oled.setCursor(115, 55);
    //    oled.print("U/L:");
    if ((statusCode) == 200) {
      oled.print("OK");
    }
    //    else {
    //      oled.print("NA");
    //    }
  }
  scale1 = false;
  scale2 = false;
  scale3 = false;
  scale4 = false;
  scale5 = false;
  scale6 = false;
  scale7 = false;
  scale8 = false;
  scale9 = false;
  scale10 = false;
  scale11 = false;
  oled.setTextColor(WHITE);
  oled.display();
}

//Monitor the serial connection from the geiger counter.
//If it is absent for > 15 seconds, display a warning
void serialmon () {
  if (millis() > (geigerTimeout + 15000)) {
    geigerConnected = false;
    geigerconnected = ("false");
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.println(Release);
    sprintf(buffer, "\nSerial NOT CONNECTED! \nCheck uart connection \nGPIO 27, # 6 on  top from L to R");
    oled.print(buffer);
    oled.display();
    APP_NAME = "serialmon: ";
    Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
    syslog.log(LOG_INFO, "Geiger not connected! - Check serial port");
    geigerTimeout = millis();
    delay(3000);
  }
}
//***Grab, store and write softSerial data from GPIO 27 to USB serial port*****
void grab() {
  if (mySerial.available()) {
    geigerdata = (mySerial.read());
    geigerSerial += char(geigerdata);
    //    Serial.write(geigerdata);
    delay(1);
  }
}
//****Process data****
void process() {
  if (geigerdata == 0x0D) {  // EOL detected
    Log.loop();
    geigerConnected = true; // Record that serial connection health is ok
    geigerconnected = ("true");
    geigerTimeout = millis();
    // Parse CPS, CPM, and uS/hr from the string
    int comma1Index = geigerSerial.indexOf(',');
    int comma2Index = geigerSerial.indexOf(',', comma1Index + 1);
    int comma3Index = geigerSerial.indexOf(',', comma2Index + 1);
    int comma4Index = geigerSerial.indexOf(',', comma3Index + 1);
    int comma5Index = geigerSerial.indexOf(',', comma4Index + 1);
    int comma6Index = geigerSerial.indexOf(',', comma5Index + 1);
    String cpsString = geigerSerial.substring(comma1Index + 1, comma2Index);
    String cpmString = geigerSerial.substring(comma3Index + 1, comma4Index);
    String sievertsString = geigerSerial.substring(comma5Index + 1, comma6Index);
    cps = cpsString.toInt();
    cpm = cpmString.toInt();
    sieverts = sievertsString.toFloat();
    webdata = geigerSerial;
    Log.print(geigerSerial);
    svyear = (sieverts * 8.76);
    averagedata(); // get data averages
    //    grabgraphdata(); // grab data for mode 5 graph
    geigerSerial = ""; // Clear the string for next time
    flashblue (); // Heartbeat pulse
    if (cps <= 5)
      flashred ();  // Flash red LED * CPS
    else
      digitalWrite(13, HIGH);
    //    grabgraphdatadelay = millis();
  }
}

// ********Setup*******
void setup() {
  pinMode(13, OUTPUT); // setup LEDs
  pinMode(0, OUTPUT); // setup LEDs
  digitalWrite(0, LOW); // setup LEDs
  digitalWrite(13, LOW); // setup LED
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  mySerial.begin(9600); // Start software serial link from Gieger
  Serial.begin(115200); // Start hardware serial console
  while (!Serial)
    ; // wait for serial port to connect. Needed for Native USB only

  oled.init();
  oled.setContrast (contrast); // reduce contrast to preserve OLED display
  Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
  Log.addPrintStream(std::make_shared<WebSerialStream>(webSerialStream));
  wifion(); // Start wifi if enabled
  Log.begin();
  APP_NAME = "setup: ";
  Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
  syslog.log(LOG_INFO, "Initial setup successful");
  cpmminavg.begin();  // Init moving average
  cpmhouravg.begin();  // Init moving average
  timerdelayavg.begin();  // Init moving average
  timerdelaydayavg.begin();  // Init moving average
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  xTaskCreatePinnedToCore(
    radmon,   /* Task function. */ // Upload to radmon.org every 60 secs using CPU0 as the HTTP GET takes some time
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
}


// ****Main Loop****
void loop() {
  serialmon();  // Monitor serial port for disconnection
  grab();  // Grab MightyOhm serial data
  process(); // Extract data and process
  server.handleClient(); // Start Web diagnostics & data server on  http://localip:80
  logstats(); // Send hourly stats to syslog server
  buttons(); // wait for button input and action
  runntp(); // update NTP if setUpdateInterval in millis has expired
  grabgraphdata(); // grab data for mode 4 histogram graph
}
