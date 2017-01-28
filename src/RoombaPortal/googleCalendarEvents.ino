/*
   Synchronously retrieves calendar events from Google Calendar using the WiFiClientSecureRedirect library
   Update the values of ssid, passwd and dstPath before use.  REFER TO DOCUMENTATION (see below for URL)

   Platform: ESP8266 using Arduino IDE
   Documentation: http://www.coertvonk.com/technology/embedded/esp8266-clock-import-events-from-google-calendar-15809
   Tested with: Arduino IDE 1.6.11, board package esp8266 2.3.0, Adafruit huzzah feather esp8266

   MIT license, check the file LICENSE for more information
   (c) Copyright 2016, Coert Vonk
   All text above must be included in any redistribution
*/

#include <WiFiClientSecureRedirect.h>

// fetch events from Google Calendar
char const * const dstHost = "script.google.com";
char const * const dstPath = "/macros/s/google_random_path__replace_with_yours_see_documentation/exec";  // ** UPDATE ME **
int const dstPort = 443;
int32_t const timeout = 5000;

// On a Linux system with OpenSSL installed, get the SHA1 fingerprint for the destination and redirect hosts:
//   echo -n | openssl s_client -connect script.google.com:443 2>/dev/null | openssl x509  -noout -fingerprint | cut -f2 -d'='
//   echo -n | openssl s_client -connect script.googleusercontent.com:443 2>/dev/null | openssl x509  -noout -fingerprint | cut -f2 -d'='
char const * const dstFingerprint = "C7:4A:32:BC:A0:30:E6:A5:63:D1:8B:F4:2E:AC:19:89:81:20:96:BB";
char const * const redirFingerprint = "E6:88:19:5A:3B:53:09:43:DB:15:56:81:7C:43:30:6D:3E:9D:2F:DE";

#define DEBUG
#ifdef DEBUG
#define DPRINT(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#define DPRINT(...)     //now defines a blank line
#define DPRINTLN(...)   //now defines a blank line
#endif

void getGoogleCalendarEventsSync() {
  DPRINT("Free heap .. "); DPRINTLN(ESP.getFreeHeap());
  bool error = true;
  WiFiClientSecureRedirect client;

  DPRINT("Alarm sync ");
  do {
    DPRINT(".");
    if (client.connect(dstHost, dstPort) != 1) {  // send connect request
      break;
    }
    DPRINT(".");
    while (!client.connected()) {  // wait until connected
      client.tick();
    }
    DPRINT(".");
    if (client.request(dstPath, dstHost, 2000, dstFingerprint, redirFingerprint) != 0) { // send alarm request
      break;
    }
    DPRINT(".");
    while (!client.response()) {  // wait until host responded
      client.tick();
    }
    DPRINT(".\n<RESPONSE>\n");
    while (client.available()) {  // read and print until end of data
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
    DPRINT("</RESPONSE>");
    client.stop();
    error = false;
  } while (0);
  DPRINTLN(error ? " error" : " done");
}
