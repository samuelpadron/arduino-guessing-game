// OLED display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Gesture sensor libraries
#include <Wire.h>
#include <paj7620.h>

#define GES_REACTIONE_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000
#define I2C_ADDRESS 0x43
#define I2C_ADDRESS2 0x44

#define OLED_RESET 1  //GPIO1

Adafruit_SSD1306 display(OLED_RESET);


// Scores
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
