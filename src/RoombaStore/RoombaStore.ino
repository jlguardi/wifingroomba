#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

/*
 * This example shows how to use store functionality
 */

typedef struct {
  char softAP_ssid[32];      /* ssid for generated AP network */
  char softAP_password[32];  /* password for generated AP network */
  char myHostname[32];       /* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
  char ssid[32];             /* ssid of WIFI network */
  char password[32];         /* password of WIFI network */
} config_t;

const config_t default_config_values = {"ESP_ap", "12345678", "esp8266", "", ""};

config_t config;

void print_config(){
  Serial.print("Recovered credentials for ");
  Serial.print(config.myHostname);
  Serial.println(":");
  Serial.print("AP: ");
  Serial.print(config.softAP_ssid);
  Serial.print(" ");
  Serial.println(config.softAP_password);
  Serial.print("WIFI: ");
  Serial.print(config.ssid);
  Serial.print(" ");
  Serial.println(strlen(config.password)>0?"********":"<no password>");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  bool ret = loadStore(); 
  print_config();
  saveStore();
}

void loop() {
  // Do nothing 
}

