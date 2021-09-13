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
#define OLED_RESET 1  //GPIO


#define LISTEN_PORT 80

const char* ssid = "NDL_24G";
const char* password = "RT-AC66U";

MDNSResponder mdns;
ESP8266WebServer server(LISTEN_PORT);
String webPage = "<h1>NDL Game</h1>";

bool startGame = false;
bool buttonClicked = false;

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);

  uint8_t error = paj7620Init();  //init Paj7620 registers
  if (error) {
    Serial.print("INIT ERROR,CODE: ");
    Serial.println(error);
  }
  
  webPageLayout();
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

  server.on("/button", [](){
    server.send(200, "text/html", webPage);
    buttonClicked = !buttonClicked;
    Serial.print("button");
    Serial.println(buttonClicked);
  });



  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  gestureHandler();
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("WiFi Test");
  display.print("Button = ");
  display.println(buttonClicked);
  display.display();
  
  delay(200);
}

void webPageLayout() {
  webPage += "<p>Press me <a href=\"button\">";
  webPage += "<button style=\"background-color:blue;color:white;\">";
  webPage += "LED</button></a></p>";

}

void gestureHandler() {
  Serial.begin(9600);
  uint8_t data = 0, data1 = 0, error;
  error = paj7620ReadReg(I2C_ADDRESS, 1, &data); // Read gesture result
  if (!error) {
    switch (data) {
      case GES_RIGHT_FLAG:
        Serial.println("Right");
        break;
      case GES_LEFT_FLAG:
        Serial.println("Left");
        break;
      case GES_UP_FLAG:
        Serial.println("Up");
        break;
      case GES_DOWN_FLAG:
        Serial.println("Down");
        break;
      case GES_FORWARD_FLAG:
        Serial.println("Forward");
        break;
      case GES_BACKWARD_FLAG:
        Serial.println("Backward");
        break;
      case GES_CLOCKWISE_FLAG:
        Serial.println("Clockwise");
      case GES_COUNT_CLOCKWISE_FLAG:
        Serial.println("Anti-clockwise");
        break;
      default:
        paj7620ReadReg(I2C_ADDRESS2, 1,&data);
        if (data == GES_WAVE_FLAG) {
          Serial.println("wave");
        } else {
          Serial.print(".");
        }
        break;
    }
  }
}
