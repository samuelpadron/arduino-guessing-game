// OLED display, gesture sensor, and WiFi libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <paj7620.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define GES_REACTIONE_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000
#define I2C_ADDRESS 0x43
#define I2C_ADDRESS2 0x44

#define OLED_RESET 1  //GPIO1

#define LISTEN_PORT 80

const char* ssid = "NDL_24G";
const char* password = "RT-AC66U";

Adafruit_SSD1306 display(OLED_RESET);

String webPage = "<h1>NDL Game</h1>";

// Game Variables
int MAX_SCORE;

int PLAYER_SCORE;
int CPU_SCORE;



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
}
