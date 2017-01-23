/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/
#include <SoftwareSerial.h>


// const char* host = "roomba";
//
// const char* ssid = "myssid";
// const char* password = "mypassword";
// const char* APssid = "Roomba";
// const char* APpassword = "password";

#define TX D1
#define RX D2
#define DD D3
SoftwareSerial SCISerial(RX,TX);

// const byte DNS_PORT = 53;
// DNSServer dnsServer;
//
// ESP8266WebServer server(80);
// const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
//
// void handleRoot() {
//   server.send(200, "text/html", "<h1>You are connected. Go to /update or /api</h1>");
// }

void SCIsetup(){
  SCISerial.begin(115200);
  pinMode(DD, OUTPUT);
  digitalWrite(DD, HIGH);
}

void SCIwrite(unsigned char val){
  SCISerial.write(val);
  delay(50);
}
void SCIwakeUp(){
  digitalWrite(DD, HIGH);
  delay(1000);
  digitalWrite(DD, LOW);
  delay(2000);
  digitalWrite(DD, HIGH);
  delay(2000);
}

void SCIsleep(){
   digitalWrite(DD, LOW);
}
/**
 * cmd    OPCODE DATA
 * Start     128
 * Baud      129    1
 * Control   130
 * Save      131
 * Full      132
 * Power     133
 * Spot      134
 * Clean     135
 * Max       136
 * Drive     137    4  // vel_high vel_low rad_high rad_low -> [-500,500]mm/s [-2000,2000]mm -> stright: 0x8000, clockwise: -1, counter-clockwise: 1
 * Motors    138    1
 * Leds      139    3
 * Song      140    2N+2
 * Play      141    1
 * Sensors   142    1
 * Dock      143
 */
enum Commands
{
  Start = 128,
  Baud,
  Control, //deprecated 130 -> use Safe instead
  Safe,
  Full,
  Power,
  Spot,
  Clean,
  Max,
  Drive,
  Motors,
  Leds,
  Song,
  Play,
  Sensors,
  Dock
};

void SCIon(){
  SCIwrite(Commands::Start);
  SCIwrite(Commands::Safe);
}

void SCIclean(){
  SCIwrite(Commands::Clean);
}

void SCIspinLeft() {
  SCIwrite(137);   // DRIVE
  SCIwrite(0x00);   // 0x00c8 == 200
  SCIwrite(0xc8);
  SCIwrite(0x00);
  SCIwrite(0x01);   // 0x0001 == spin left
}

void handleAPI() {
  float pos = 1024 * (1 - server.arg("pwr").toFloat()/100.0);
  digitalWrite(BUILTIN_LED, LOW);  // turn on LED with voltage HIGH
  delay(1000);                      // wait one second
  analogWrite(BUILTIN_LED, pos);
  if(server.arg("on") == "1"){
    SCIwakeUp();
    SCIon();
    //SCIclean();
    SCIspinLeft();
  }
  if(server.arg("on") == "0"){
    SCIsleep();
  }
  std::string resp = "";
  resp += "<h1>PWR:" + analogRead(BUILTIN_LED);
  resp += "%</h1><br/>";
  resp += "<h1>STATUS: " + digitalRead(DD);
  resp += "</h1>";
  server.send(200, "text/html", resp.c_str());

}

void RoombaSetup(void){
  Serial.begin(115200); // Serial debug interface
  pinMode(BUILTIN_LED, OUTPUT);
  SCIsetup();
}
