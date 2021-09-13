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

#define KEYA D3
#define KEYB D4
#define OLED_RESET 1  //GPIO1


#define LISTEN_PORT 80

const char* ssid = "NDL_24G";
const char* password = "RT-AC66U";

MDNSResponder mdns;
ESP8266WebServer server(LISTEN_PORT);
String webPage = "<h1>NDL Game</h1>";

bool ledOn = false;

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(KEYA, INPUT);
  pinMode(KEYB, INPUT);
  
  digitalWrite(LED_BUILTIN, ledOn);

  WiFi.begin(ssid, password);
  Serial.println("Start connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(". IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  // web page handlers
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println(webPage);
  display.display();
  delay(200);
}
