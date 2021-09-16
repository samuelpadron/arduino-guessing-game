// OLED display, gesture sensor, and WiFi libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <paj7620.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LOLIN_I2C_MOTOR.h>

#define GES_REACTIONE_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000
#define I2C_ADDRESS 0x43
#define I2C_ADDRESS2 0x44
#define OLED_RESET 1  //GPIO
#define LISTEN_PORT 80
#define GESTURE_DELAY 2000
#define MOTOR_DELAY 2000
#define PWMFREQUENCY 1000


bool gestureHandling = false;
long gestureLastTime = 0;
long motorLastTime = 0;

MDNSResponder mdns;
ESP8266WebServer server(LISTEN_PORT);
LOLIN_I2C_MOTOR motor(DEFAULT_I2C_MOTOR_ADDRESS);

Adafruit_SSD1306 display(OLED_RESET);

const char* ssid = "NDL_24G";
const char* password = "RT-AC66U";

String webPage = "<h1>NDL Guessing Game</h1>";

int playerScore = 0, rewardScore = 5;
bool startGame = false;
bool correctAnswer = false;
bool newChoice = true;
const String directions[] = {"UP", "RIGHT", "DOWN", "LEFT"};
String playerGuess, correctChoice;

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(115200);

   while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) {
    motor.getInfo();
  }
  
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

  webPageHandlers();
 
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  if(millis() - GESTURE_DELAY > gestureLastTime) {
    roundSetup();
    displayMainScreen();
    gestureLastTime += GESTURE_DELAY;
  }
}

void webPageHandlers() {
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });


  server.on("/button", [](){
    server.send(200, "text/html", webPage);
    playerScore = 0;
    Serial.print("player score reset");
  });

  server.on("/button5", [](){
    server.send(200, "text/html", webPage);
    rewardScore = 5;
  });

  server.on("/button10", [](){
    server.send(200, "text/html", webPage);
    rewardScore = 10;
  });

  server.on("/button15", [](){
    server.send(200, "text/html", webPage);
    rewardScore = 15;
  });
}

void webPageLayout() {
  webPage += "<p>PLAYER SCORE <a href=\"button\">";
  webPage += "<button style=\"background-color:red;color:white;\">";
  webPage += "RESET</button></a></p>";
  webPage += "<p>GOAL SCORE <a href=\"button5\">";
  webPage += "<button style=\"background-color:blue;color:white;\">";
  webPage += "5</button></a> <a href=\"button10\">";
  webPage += "<button style=\"background-color:blue;color:white;\">";
  webPage += "10</button></a> <a href=\"button15\">";
  webPage += "<button style=\"background-color:blue;color:white;\">";
  webPage += "15</button></a></p>";
}

void roundSetup(){
  if(newChoice) {
   correctChoice = directions[random(3)]; 
  }
   gestureHandler();
   guessHandler();
}

void displayMainScreen() {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println("??????????");
    display.println("Guess the Direction");
    display.println("??????????");
    display.println("GUESSED: ");
    answerDisplay();
    display.display();
}

void answerDisplay(){
     if (correctAnswer) {
      display.println("CORRECTLY");
     }
     else {
      display.println("GUESSING...");
     }
}

void guessHandler() {
  if ( correctChoice == playerGuess) {
      correctAnswer = true;
      playerScore += 1;
      if( playerScore == rewardScore) {
        rewardPlayer();
      }
      newChoice = true;
   }
   else {
      correctAnswer = false;
      newChoice = false;
   }
   playerGuess = "GUESSING"; //This makes it so that the new choice by the program does not match the player's last choice and gives him a freebie. 
}

void rewardPlayer(){
  motor.changeFreq(MOTOR_CH_BOTH, PWMFREQUENCY);
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_CCW);
  if(millis() - MOTOR_DELAY > motorLastTime) {
    motor.changeDuty(MOTOR_CH_A, 50);
    motorLastTime += MOTOR_DELAY;
  }
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_STANDBY);

  Serial.println("Moving motor");
}

void gestureHandler() {
  Serial.begin(9600);
  uint8_t data = 0, data1 = 0, error;
  error = paj7620ReadReg(I2C_ADDRESS, 1, &data); // Read gesture result
  if (!error) {
    switch (data) {
      case GES_UP_FLAG:
        playerGuess = "UP";
        break;
      case GES_RIGHT_FLAG:
         playerGuess = "RIGHT";
        break;
      case GES_DOWN_FLAG:
        playerGuess = "DOWN";
        break;
      case GES_LEFT_FLAG:
        playerGuess = "LEFT";
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
  Serial.println("Guess: " + playerGuess + ", player score:" + playerScore);
  Serial.println("Correct: " + correctChoice + ", reward score: " + rewardScore);
}
